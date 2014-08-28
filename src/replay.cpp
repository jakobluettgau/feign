#include <stdio.h>
#include <list>
#include <iostream>

#include "replay.h"
#include "plugins.h"

#include "modules/helper/helper.h"

// buffer settings
std::list<Activity*> buffer;
int capacity = 10000;				// max capacity
double stockpile = 0.5;			// threshold to kickoff preload
//int lookahead = 1;				// TODO: setable by plugins, temporarly global, see common.h, main.h
int provider_depleted = 0;		// providers depleted?
int buffer_depleted = 0;		// providers depleted?

// replay flags
//int precreation = 1;
int precreation = 0;


// replay statistics
int provide_count = 0;
int filter_count = 0;
int mutate_count = 0;
int swap_count = 0;
int insert_count = 0;
int precreate_check_count = 0;
int replay_count = 0;


/**
 * helper for easy nanosecond sleeping  (accuracy more around 70k ns?)
 */
void nsleep(long ns) {
	static long spin = 9;

	if ( ns > 70000 )
	{
		ns -= 65000;
		struct timespec req = {
			.tv_sec = 0,
			.tv_nsec = ns
		};

		struct timespec rem, *a = &req, *b = &rem;

		while ( nanosleep(a,b) && errno == EINTR) {
			struct timespec *tmp = a;
			a = b;
			b = tmp;
		}
	} else {
		while ( ns > 0 ) {
			ns -= spin;
		}
	}
}



/**
 * preload phase that calls all context unaware filters and modifiers and
 * populates the buffer
 *
 * @return	status	1 = providers depleted, 0 = not depleted
 */
int preload() {
	//printf("preload()\n");

	int preload_count = 0;

	//std::cout << "buffer.size=" << buffer.size() << "\n";

	feign_log(4, "bufsize: %d\n", buffer.size() );  

	int firstrun = 1;
	Activity * a;
	//while ( (preload_count < lookahead && a != NULL) || firstrun ) {
	while ( buffer.size() < lookahead && a != NULL )  {
		firstrun = 0;
		a = activity_provide();
		if ( a != NULL )
		{
			provide_count++;
			preload_count++;



			feign_log(9, "Preload Activity %p: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a, a->status, a->provider, a->offset, a->layer, a->size, a->rank);

			// filter
			Activity * filtered = activity_filter(a);
			if ( filtered == NULL ) {
				filter_count++;
				feign_log(9, "$Destroy: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a->status, a->provider, a->offset, a->layer, a->size, a->rank);
				activity_destroy(a);
				//printf("- - -\n");
				a = activity_provide();
				continue;
			}

			// mutate
			Activity * mutated = activity_mutate(a);
			if ( mutated->status == FEIGN_STATUS_MUTATED )
			{
				mutate_count++;
				// swapped/filtered?
				if ( mutated != a ) {
					swap_count++;
					// filtered?
					if ( mutated == NULL ) {
						filter_count++;
						// TODO: decide if to destroy
						feign_log(9, "$Destroy: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a->status, a->provider, a->offset, a->layer, a->size, a->rank);
						activity_destroy(a);
						printf("- - -\n");
						a = activity_provide();
						continue;
					}

					//printf("- - -\n");
					a = mutated;
				}
			}

			buffer.push_back(a);

			// end of cycle, provide next
			//printf("- - -\n");
		} else {
			// announce provder depletion
			provider_depleted = 1;
		}
	}

	//std::cout << "buffer.size=" << buffer.size() << "\n";

	return provider_depleted;
}


/**
 * replay phase, for context aware filters and modifiers and for replay
 * workoff buffer, cleanup
 *
 * return 1 = buffer depleted
 */
int replay() {
	//printf("replay()\n");

	double fillstate = buffer.size() / (double)capacity;
	//printf("fillstate: %f\n", fillstate);


	//while ( (buffer.size() > lookahead) || (buffer.size() && provider_depleted) )
	//	{

		Activity * a = buffer.front();

/*
typedef struct Activity {
	int status;  // like to discard an activity without reorganisation penalty
	int provider;
	long long offset;
	int layer;
	int size;
	void * data;
	Statistic * stats;
	int rank;
} Activity;
*/

		feign_log(9, "Activity %p: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a, a->status, a->provider, a->offset, a->layer, a->size, a->rank);
		

		//provide_count++;

		// context aware filter
		/* *
		Activity * filtered = activity_filter(a);
		if ( filtered == NULL ) {
			filter_count++;
			activity_destroy(a);
#define DEBUG(a) printf("%-30s %3d %s:%d\n", a, -1, __FILE__, __LINE__)
			printf("- - -\n");
			a = activity_provide();
			continue;
		}
		/* */

		int filtered = activity_filter_context(buffer.begin(), buffer.end(), buffer);
		if ( filtered == 1 ) {
			filter_count++;
			activity_destroy(a);
			//printf("- - -\n");
			a = activity_provide();
			//continue;
		}


		print_replay_buffer();

		int mutated = activity_mutate_context(buffer.begin(), buffer.end(), buffer);
		if ( mutated == 1 ) {
			mutate_count++;
		}

		if ( mutated == 3 ) {
			filter_count++;
			activity_destroy(a);
			//printf("- - -\n");
			a = activity_provide();
			//continue;
		}

		print_replay_buffer();


		feign_log(3,"offset: %ld\n", a->offset);
		long actual_sleep = a->offset;
		struct timespec start;
		struct timespec end;

		//start_timer(&start);
		nsleep(actual_sleep);
		//stop_timer(&start, &end, -1);


		if ( precreation ) {
			// precreation_check
			activity_precreate_check(a);
			precreate_check_count++;
		} else {
			// replay
			activity_replay(a);
			replay_count++;
		}




		// destroy
		activity_destroy(a);

		buffer.pop_front();

		// end of cycle, provide next
		//printf("- - -\n");

	//}


	if ( buffer.size() == 0 ) {
		buffer_depleted = 1;
		precreation = 0;
	}


	return buffer_depleted;
}



void print_replay_buffer() {
	if ( loglevel >= 15 ) {
		feign_log(15, "== Current Buffer =====================================================================================\n");
		for (auto it=buffer.begin(); it != buffer.end(); ++it) {
			Activity * a = (*it);
			feign_log(15, "= Activity %p: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a, a->status, a->provider, a->offset, a->layer, a->size, a->rank);
			//activity_replay(a);
		}
		feign_log(15, "=======================================================================================================\n");
	}
}


void reset_buffer_flags() {
	provider_depleted = 0;
	buffer_depleted = 0;
}



/**
 * print the different activity counters
 */
void replay_manager_print_stats() {
	printf("Replay Statistics\n");
	printf("Provided:   %-5d\n", provide_count);
	printf("Filtered:   %-5d\n", filter_count);
	printf("Mutated:    %-5d\n", mutate_count);
	printf("'-Swapped:  %-5d\n", swap_count);
	printf("'-Inserted: %-5d\n", insert_count);
	printf("Replayed:   %-5d\n", precreate_check_count);
	printf("Replayed:   %-5d\n", replay_count);

	printf("Option: lookahead = %d\n", lookahead);
	printf("\n");
}

void reset_replay_stats() {
	// replay statistics
	provide_count = 0;
	filter_count = 0;
	mutate_count = 0;
	swap_count = 0;
	insert_count = 0;
	precreate_check_count = 0;
	replay_count = 0;
}
