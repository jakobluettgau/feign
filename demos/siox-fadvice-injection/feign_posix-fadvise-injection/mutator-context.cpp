#include <stdio.h>
#include <iostream>
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
	.intents = FEIGN_MUTATOR_CONTEXT | FEIGN_DESTROYER,
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

/**
 *  As a reminder for the relevant declarations and definitons.
 **
// feign activity
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

// posix activity
typedef struct posix_activity {
	int type;
	void * data;
	int ret;
} posix_activity;

// posix_fadvise data
struct posix_posix_fadvise_struct
{
	int  fd;
 	off_t  offset;
 	off_t  len;
 	int  advise;
 	int ret;
};
typedef struct posix_posix_fadvise_struct posix_posix_fadvise_data;

 **
 * @return	pointer to a newly craeted activity
 */
Activity * create_posix_fadvise(int fd, off_t offset, off_t len, int advise, int ret) 
{
	// allocate memory for the activity
    Activity * activity = (Activity *) malloc(sizeof(Activity));
    posix_activity * sub_activity = (posix_activity *) malloc(sizeof(posix_activity));
	posix_posix_fadvise_data * d = (posix_posix_fadvise_data *) malloc(sizeof(posix_posix_fadvise_data));

	// set values
	d->fd = fd;
	d->offset = offset;
	d->len = len;
	d->advise = advise;
	d->ret = ret;

    sub_activity->type = POSIX_posix_fadvise;
    sub_activity->data = (void *) d;

    activity->layer = layer_id;
    activity->offset = 0;
    activity->data = (void *) sub_activity;
    activity->provider = plugin.instance_id;

	return activity;
}



int mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	FEIGN_LOG(3, "mutate_context(): inject fadvise before lseek");

	Activity * activity = (*iter);

	// how many activities in the future do we consider?
	int check_num = 5;

// TURN (original):

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

// TO (original with injected fadvise):

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

	int stopit = 0;


	while ( iter != end && check_num > 0 )
	{
		feign_log(4, "m: activity->layer = %d, check_num = %d\n", activity->layer, check_num);

		if ( activity->layer == layer_id ) {
			posix_activity * sub_activity = (posix_activity*)activity->data;

			void * data = sub_activity->data;

			switch ( sub_activity->type ) {
				case POSIX_write:
					{
						posix_write_data * d = (posix_write_data*) data;
						FEIGN_LOG(1, "write found!");

						Activity * a = create_posix_fadvise(0,0,0,0,0);

	feign_log(9, "## MUT: status=%d, provider=%d, offset=%d, layer=%d, size=%d, rank=%d\n", a->status, a->provider, a->offset, a->layer, a->size, a->rank);

					
						std::cout << "list size=" << list.size() << std::endl;

						// insert inserts before
						if ( check_num < 5 )
						{
							list.insert(iter, a);
						}

						feign_log(1, "advice added! %p\n", a);

					}
					break;


				case POSIX_lseek:
					{
						posix_lseek_data * d = (posix_lseek_data*) data;
						FEIGN_LOG(1, "lseek found!");

						Activity * a = create_posix_fadvise(0,0,0,0,0);
						// insert inserts after
						
						//list.insert(iter, a);
						feign_log(1, "advice added! %p\n", a);
					}
					break;
			}


			if ( stopit ) {
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

