#include <stdio.h>

// include feign helpers for easier plugin development
//////////////////////////////////////////////////////
#include <feign.h>

// layer includes
/////////////////
#include "datatypes.h"
#include "wrapper.h"


int layer_id = 13;


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "posix-replayer",
	.version = NULL,
	.intents = FEIGN_REPLAYER,
};

// implement handlers
/////////////////////
/**
 * Point of entry for plugins, provided to set up the plugin, register hooks
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

	// announce features and request further action
	return &plugin;
}

// replay
/////////
Activity * replay(Activity * activity) {
	DEBUG("replay");

	if ( activity->layer == layer_id ) {

		//dump_map();

		posix_activity * pactivity = (posix_activity*)activity->data;

		// a simple way to produce action
		switch ( pactivity->type ) {
			case POSIX_READ:
				DEBUG("read()");
				posix_read(pactivity->data);
				break;

			case POSIX_FREAD:
				DEBUG("fread()");
				posix_fread(pactivity->data);
				break;

			case POSIX_WRITE:
				DEBUG("write()");
				posix_write(pactivity->data);
				break;

			case POSIX_OPEN:
				DEBUG("open()");
				posix_open(pactivity->data);
				break;

			case POSIX_FOPEN:
				DEBUG("fopen()");
				posix_fopen(pactivity->data);
				break;

			case POSIX_CLOSE:
				DEBUG("close()");
				posix_close(pactivity->data);
				break;

			default:
				DEBUG("unknown type");
		}
	} else {
		DEBUG("!");
	}



	return NULL;
}


/**
 * When the finalize hook has been registered, feign calls this to give the
 * plugin a chance to free resources and possibly dump results.
 *
 * Could be only called when also registered OR should it be required?
 */
int finalize() {

	return 0;
}

