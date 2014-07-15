#include <stdio.h>

#include <feign.h>

// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "sample-filter",
	.version = NULL,
	.intents = FEIGN_FILTER,
};

// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello sample plugin!\n");

	// return plugin
	return &plugin;
}


Activity * filter(Activity * activity) {
	CDEBUG("filter");
	if ( activity->layer == 1 )
		return NULL;
	else
		return activity;
}


int finalize() {

	return 0;
}

