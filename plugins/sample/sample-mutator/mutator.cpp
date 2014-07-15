#include <stdio.h>

#include <feign.h>

// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "sample-mutator",
	.version = NULL,
	.intents = FEIGN_MUTATOR,
};

// implement handlers
/////////////////////
/**Point of entry for plugins, provided to set up the plugin, register hooks
 * and to announce capabilities and handlers so feign makes use of the plugin
 * as desired.
 *
 * The only mandatory function.
 */
Plugin * init() {
	// maybe provide an ENUM Bitmap Type to easilly announce capabilities
	// e.g. FEIGN_PLUGIN_SUCCESS && FEIGN_PLUGIN_PROVIDER && FEIGN_PLUGIN_REPLAYER
	// to say: init was completed succesfully, i can provide and replay tracedata
	// please call the appropiate handlers

	printf("Hello sample plugin!\n");
	// gonna be provider and replayer
	// but provider should negotiate identifier

	// announce features and request further action
	return &plugin;
}


Activity * mutate(Activity * activity) {
	CDEBUG("mutate");
	// This could for instance:
	// - change some members of the activity
	// - exchange the given activity with another one
	// - filter

	if ( activity->layer == 2 )
	{
		activity->layer = 3;
		activity->status = FEIGN_STATUS_MUTATED;
	}

	return activity;
}

/**When the finalize hook has been registered, feign calls this to give the
 * plugin a chance to free resources and possibly dump results.
 *
 * Could be only called when also registered OR should it be required?
 */
int finalize() {

	return 0;
}

