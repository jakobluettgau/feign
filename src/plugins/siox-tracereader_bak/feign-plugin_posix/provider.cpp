#include <stdio.h>
#include <stdlib.h>

// include feign helpers for easier plugin development
//////////////////////////////////////////////////////
#include <feign.h>

// layer includes
/////////////////
#include "datatypes.h"


int layer_id = 13;

posix_open_data open_data[] = {
	/* name, flags, mode, ret */
	{ "posix_tmp_1", 0, 0, 6 },
	{ "posix_tmp_2", 0, 0, 12 },
};

posix_read_data read_data[] = {
	/* fd, count, ret */
	{ 6, 1000, 0 },
	{ 6, 10000, 0 },
	{ 12, 1000, 0 },
	{ 12, 10000, 0 },
};


posix_activity sub_activities[] = {
	/* type, data* */
	{ POSIX_OPEN, &open_data[0] },
	{ POSIX_OPEN, &open_data[1] },
	{ POSIX_READ, &read_data[0] },
	{ POSIX_READ, &read_data[0] },
	{ POSIX_WRITE, NULL },
	{ POSIX_WRITE, NULL },
	{ 2, NULL },
	{ 3, NULL },
};


int count = (sizeof sub_activities / sizeof sub_activities[0]);
int num_provided = 0;


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "posix-provider",
	.version = NULL,
	.intents = FEIGN_PROVIDER,
};

// implement handlers
/////////////////////
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


void * create_subactivity(long offset, int type) {

	// a simple way to produce action
	switch ( type ) {
		case POSIX_READ:
			DEBUG("read()");
			break;

		case POSIX_WRITE:
			DEBUG("write()");
			break;

		case POSIX_OPEN:
			DEBUG("open()");
			break;

		case POSIX_CLOSE:
			DEBUG("close()");
			break;

		default:
			DEBUG("unknown type");
	}



	Activity * sa = (Activity *) malloc(sizeof(Activity));

	return sa;
}


Activity * create_activity(long offset, int type) {
	Activity * a = (Activity *) malloc(sizeof(Activity));

	//posix_activity * data = (posix_activity *) malloc(sizeof(posix_activity));
	//posix_write_data * posix_data = (posix_write_data *) malloc(sizeof(posix_write_data));
	//data->type = type;

	a->layer = layer_id;
	a->offset = offset;
	a->data = &sub_activities[type];
	a->provider = plugin.instance_id;

	return a;
}

Activity * reset(Activity * activity) {
	CDEBUG("reset");
	num_provided = 0;
}


Activity * provide(Activity * activity) {
	CDEBUG("provide");
	activity = NULL;
	if ( 0 <= num_provided && num_provided < count ) {
		activity = create_activity(0, num_provided);
		num_provided++;
	}
	return activity;
}


Activity * destroy(Activity * activity) {
	CDEBUG("destroy");

	if ( activity->provider == plugin.instance_id ) {
		if ( activity->data == NULL ) {

		}
		free(activity);
		return NULL;
	} else {
		return activity;
	}
}




int finalize() {

	return 0;
}
