#include <errno.h>
#include <fcntl.h> // for posix fadvise
#include <iostream>
#include <libgen.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <feign.h>
#include "posix/datatypes.h"



// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "whitelist-rank",
	.version = NULL,
	//.intents = FEIGN_MUTATOR_CONTEXT | FEIGN_FILTER_CONTEXT | FEIGN_DESTROYER,
	.intents = FEIGN_FILTER,
};

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


int blacklisted_rank = -1;
int whitelisted_rank = -1;

// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello from %s!\n", plugin.name);

    char const * str_whitelisted_rank = get_dat_env("FEIGN_WHITELISTED_RANK", "-1");
	whitelisted_rank = strtol(str_whitelisted_rank,NULL,10);

    char const * str_blacklisted_rank = get_dat_env("FEIGN_BLACKLISTED_RANK", "-1");
	blacklisted_rank = strtol(str_blacklisted_rank,NULL,10);


	printf("feign-whitelisted-rank options: whitelisted_rank = %d \n", whitelisted_rank);

	// return plugin
	return &plugin;
}




Activity * filter(Activity * activity) {
	FEIGN_LOG(1, "filter-rank");

	if ( activity->rank != whitelisted_rank ) 
	{	
		FEIGN_LOG(2, "activity-filtered because rank matched");
		return NULL;
	}

	return activity;
}



int finalize() {

	return 0;
}

