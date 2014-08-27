#include <stdio.h>

// include feign helpers for easier plugin development
//////////////////////////////////////////////////////
#include <feign.h>

// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "test-helper",
	.version = NULL,
	.intents = FEIGN_UNDEFINED,
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

	printf("Hello from %s\n", __FILE__);
	// gonna be provider and replayer
	// but provider should negotiate identifier

	feign_log(2, "testing loglevel 2");
	feign_shared_byte_buffer(3);


	// announce features and request further action
	return &plugin;
}

/**When the finalize hook has been registered, feign calls this to give the
 * plugin a chance to free resources and possibly dump results.
 *
 * Could be only called when also registered OR should it be required?
 */
int finalize() {

	return 0;
}

