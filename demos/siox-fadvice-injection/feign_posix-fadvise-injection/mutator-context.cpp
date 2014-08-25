#include <stdio.h>
#include <list>

#include <feign.h>
#include "posix/datatypes.h"

// some data
////////////


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "fadvise-injector",
	.version = NULL,
	.intents = FEIGN_MUTATOR_CONTEXT,
};


int layer_id = 13;


// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello sample plugin!\n");

	// return plugin
	return &plugin;
}


int mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	FEIGN_LOG(3, "mutate_context(): inject fadvise before lseek");

	Activity * activity = (*iter);

	// how many activities in the future do we consider?
	int check_num = 5;

// TURN:

// [...]
// lseek(3, 37552128, SEEK_SET)            = 37552128
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f19b28c1000, 1024, [00000000000000000000000010000000...]) = 0
// read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
// lseek(3, 37564416, SEEK_SET)            = 37564416
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f19b28c4000, 1024, [00000000000000000000000010000000...]) = 0
// read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
// lseek(3, 37576704, SEEK_SET)            = 37576704
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f19b28c7000, 1024, [00000000000000000000000010000000...]) = 0
// read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
// lseek(3, 37588992, SEEK_SET)            = 37588992
// nanosleep({0, 100000}, NULL)            = 0
// [...]

// TO:

// [...]
// fadvise64(3, 24563712, 1024, POSIX_FADV_WILLNEED) = 0
// lseek(3, 24563712, SEEK_SET)            = 24563712
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f9d9c732000, 1024, [10000000000000000000000010000000...]) = 0
// read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
// fadvise64(3, 24576000, 1024, POSIX_FADV_WILLNEED) = 0
// lseek(3, 24576000, SEEK_SET)            = 24576000
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f9d9c735000, 1024, [10000000000000000000000010000000...]) = 0
// read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
// fadvise64(3, 24588288, 1024, POSIX_FADV_WILLNEED) = 0
// lseek(3, 24588288, SEEK_SET)            = 24588288
// nanosleep({0, 100000}, NULL)            = 0
// mincore(0x7f9d9c738000, 1024, [10000000000000000000000010000000...]) = 0
// [...]


	while ( iter != end && check_num > 0 )
	{
		feign_log(4, "m: activity->layer = %d, check_num = %d\n", activity->layer, check_num);

		if ( activity->layer == layer_id ) {
			posix_activity * sub_activity = (posix_activity*)activity->data;

			switch ( sub_activity->type ) {
				case POSIX_lseek:
					{
						FEIGN_LOG(1, "lseek found!");
					}
					break;
			}

		}


		iter++;
		/*
		activity = (*iter);
		// insert after 4 if not followed by 5
		if ( last == 4 && activity->layer != 5 ) {
			list.insert(iter, &a0);
		}
		*/

		check_num--;
	}


	//activity = (*it);
	//printf("f: activity->layer = %d\n", activity->layer);

	/*
	if ( activity->layer == 1 )
		return NULL;
	else
		return activity;
	*/

	return 0;
}


int finalize() {

	return 0;
}

