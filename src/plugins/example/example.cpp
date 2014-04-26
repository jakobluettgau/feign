#include <feign.h>

// information about the plugin
Plugin plugin = {
	.name = "Example-Provider-Replayer",
	.version = NULL,
};

// information about a layer
Layer layer = {
	.name = "ExampleLayer",
	.intents = FEIGN_PROVIDER | FEIGN_REPLAYER,
}

int init(int argc, char *argv[])
	// registration of plugin and layer
	feign_register_plugin(&plugin);
	feign_register_layer(&layer);
	return o;
}

// expected because of FEIGN_PROVIDER
Activity * provide(Activity * activity) {
	return create_activity_from_datasource();
}

Activity * destroy(Activity * activity) {
	if ( activity->provider == plugin.id ) {
		// this plugin created the activity, so it needs to destroy it
		free(Activity);
		return NULL;
	} else {
		return activity;
	}
}

// expected because of FEIGN_REPLAYER
Activity * replay(Activity * activity) {
	if ( activtiy->layer == layer.id ) {
		do_something();
		return NULL;
	} else {
		return activity;
	}
}
