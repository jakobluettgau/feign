#include <stdio.h>
#include <iostream>
#include <list>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h> // for posix fadvise

#include <feign.h>
#include "posix/datatypes.h"



// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "posix-coalescing optimizer",
	.version = NULL,
	//.intents = FEIGN_MUTATOR_CONTEXT | FEIGN_FILTER_CONTEXT | FEIGN_DESTROYER,
	.intents = FEIGN_MUTATOR_CONTEXT | FEIGN_DESTROYER,
};

int layer_id = 13;



char const * get_dat_env(char const * name, char const * fallback) {
    char const * value;
    value = getenv (name);
    if (! value) {
        value = fallback;
    }
    else {
        printf ("%s = %s\n", name, value);
    }

    return value;
}

long max_chunksize = 1024;

// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello from %s!\n", plugin.name);

    char const * str_chunksize = get_dat_env("FEIGN_COALESCING_CHUNKSIZE", "1024");
	max_chunksize = strtol(str_chunksize,NULL,10);

	if ( 0 == max_chunksize ) {
		max_chunksize = 1024*1024*4;	
	}

	feign_log(0, "using max_chunksize=%d  (input: %s) \n", max_chunksize, str_chunksize);

	// return plugin
	return &plugin;
}




int filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	
	FEIGN_LOG(1, "filter_context(): remove marked");

	int filterthis = 0;


	Activity * activity = (*iter);


	if ( activity->layer == layer_id ) 
	{
		posix_activity * sub_activity = (posix_activity*)activity->data;
		void * data = sub_activity->data;
		
		if ( sub_activity->type == POSIX_write && activity->status == 77 )
		{
			posix_write_data * d = (posix_write_data*) data;
			feign_log(9, "rm: posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", d->fd, d->buf, d->count, d->ret); 	

			//filterthis = 1;
		}
	}


	if ( filterthis )
		return 1;
	else
		return 0;
}


int mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
// TURN (original):
// [...]
// write(fd, buf, 10);
// write(fd, buf, 10);
// write(fd, buf, 10);
// write(fd, buf, 10);
// write(fd, buf, 10);
// [...]

// TO (original with injected fadvise):
// [...]
// write(fd, buf, 50);
// [...]

	FEIGN_LOG(1, "mutate_context(): coalescing");

	Activity * activity = (*iter);
	Activity * first = NULL;
	posix_activity * sub_activity = NULL;
	posix_write_data * d = NULL;
	posix_write_data * df = NULL;

	if ( iter != end ) {

		if ( activity->layer == layer_id ) {
			
			sub_activity = (posix_activity*)activity->data;

			if ( sub_activity->type == POSIX_write ) {

				// ok is a write, remember this
				first = activity;
				df = (posix_write_data*) sub_activity->data;

				iter++;

				if ( iter == end )
				{
					// last activity nothing to collapse
					return 0;
				}

				// let activity point to updated iterator
				activity = (*iter);

				
				if ( activity->layer == layer_id ) {
					// cast subactivity to get actual data
					sub_activity = (posix_activity*)activity->data;

					if ( sub_activity->type == POSIX_write ) {

						// ok, also write
						d = (posix_write_data*) sub_activity->data;

						// compare
						int sum_count = d->count + df->count;

						if ( d->fd == df->fd && sum_count < max_chunksize ) {
							FEIGN_LOG(1, "COOL, filehandles match, and smalle max chunk");

							first->status = FEIGN_STATUS_SKIP;
							
							d->count = sum_count;
							first->offset = 0;
							activity->offset = 0;

							return 3;

						}

					}

				}


			}

		}


	}


	/*
	Activity * activity = (*iter);
	Activity * first = NULL;
	posix_write_data * df = NULL;



	if ( iter != end )
	{

		if ( iter == end )
			return 0;

		activity = (*iter);
	
		if ( activity->layer == layer_id ) {

			posix_activity * sub_activity = (posix_activity*)activity->data;

			if ( sub_activity->type == POSIX_write && activity->status != FEIGN_STATUS_SKIP )
			{

				if ( first == NULL ) {
					first = activity;
					// make sure to insert only once
					activity->status = 99;  // to mark also in trace

					// quick access to first data
					void * data = sub_activity->data;
					posix_write_data * d = (posix_write_data*) data;
					df = d;
					

					feign_log(1, "§ posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", d->fd, d->buf, d->count, d->ret); 	


				} else {
					// not first write


					void * data = sub_activity->data;
					posix_write_data * d = (posix_write_data*) data;

					if (
							df->fd == d->fd 
						&& (df->count+d->count) < max_chunksize 
					) {
						FEIGN_LOG(1, ":: doing merge");
						// writes match, (no seek .. inbetween)
						activity->status = FEIGN_STATUS_SKIP;
						df->count += d->count;
						mutated = 3;
					}


					Activity * a = activity;
					feign_log(1, " /     Activity %p: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", 
							first, first->status, first->provider, first->offset, first->layer, first->size, first->rank); 
					feign_log(1, "/$ df: posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", df->fd, df->buf, df->count, df->ret); 	
					feign_log(1, "\\$     posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", d->fd, d->buf, d->count, d->ret); 	
					feign_log(1, " \\     Activity %p: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", 
							a, a->status, a->provider, a->offset, a->layer, a->size, a->rank); 



				}
		
			} else {
				// no write, chain interrupted	
				FEIGN_LOG(1, "### Chain interrupted!");
				break;
			}
		}

		//iter++;
	}
	*/	

	return 0;
}

Activity * destroy(Activity * activity) {
    FEIGN_LOG(3, "destroy");
    feign_log(9, "destroy: %p\n", activity);

	Activity * a = activity;
	feign_log(9, "$Destroy: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a->status, a->provider, a->offset, a->layer, a->size, a->rank);

    if ( activity->provider == plugin.instance_id ) {
		FEIGN_LOG(3, "I destroy!");
        if ( activity->data != NULL ) {
            posix_activity * sub_activity = (posix_activity *)(activity->data);

            if ( sub_activity->data != NULL ) {
				feign_log(9, "free(sub->data: %p)\n", activity);
                free(sub_activity->data);
            } else {
            }
			feign_log(9, "free(sub: %p)\n", activity);
            free(sub_activity);
        } else {
        }
		feign_log(9, "free(activity: %p)\n", activity);
        free(activity);
        return NULL;
    } else {
        return activity;
    }

    return NULL;
}

int finalize() {

	return 0;
}

