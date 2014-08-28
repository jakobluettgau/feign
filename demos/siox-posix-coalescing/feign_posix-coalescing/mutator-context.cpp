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
		max_chunksize = 1024*4;	
	}

	feign_log(0, "using max_chunksize=%d  (input: %s) \n", max_chunksize, str_chunksize);

	// return plugin
	return &plugin;
}




int filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	
	FEIGN_LOG(5, "filter_context(): remove marked");
	

	Activity * activity = (*iter);

	if ( activity->layer == layer_id ) 
	{
		posix_activity * sub_activity = (posix_activity*)activity->data;
		void * data = sub_activity->data;
		
		if ( sub_activity->type == POSIX_write && activity->status != 77 )
		{
			posix_write_data * d = (posix_write_data*) data;
			feign_log(9, "posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", d->fd, d->buf, d->count, d->ret); 	
		}
	}

	int filterthis = 0;

	if ( filterthis )
		return 1;
	else
		return 0;
}

int uninterrupted_write = 0;

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

	int lasttype = -1;

	FEIGN_LOG(5, "mutate_context(): inject fadvise before lseek");

	Activity * activity = (*iter);

	if ( iter != end )
	{
		iter++;

		if ( iter == end )
			return 0;

		activity = (*iter);
	
		if ( activity->layer == layer_id ) {

			posix_activity * sub_activity = (posix_activity*)activity->data;

			if ( sub_activity->type == POSIX_write && activity->status != 77 )
			{
				// make sure to insert only once
				activity->status = 77;

				// get lseek data
				void * data = sub_activity->data;
				posix_write_data * d = (posix_write_data*) data;
		
				feign_log(9, "posix_write_data: fd=%d, buf=%d, count=%d, ret=%d \n", d->fd, d->buf, d->count, d->ret); 	

				return 0;
			}
		}
	}
			




		/*
		activity = (*iter);
		// insert after 4 if not followed by 5
		if ( last == 4 && activity->layer != 5 ) {
			list.insert(iter, &a0);
		}
		*/

	



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

