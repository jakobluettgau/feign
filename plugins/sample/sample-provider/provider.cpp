#include <stdio.h>


// include feign helpers for easier plugin development
//////////////////////////////////////////////////////
#include <feign.h>



// Activity:  {  status , provider , offset , layer , size , data* , stats* }
Activity a0 = {  0      , -1       , 1      , 0     , 0    , NULL  , NULL }; // sample
Activity a1 = {  0      , -1       , 1      , 0     , 0    , NULL  , NULL }; // sample
Activity a2 = {  0      , -1       , 0      , 1     , 0    , NULL  , NULL }; // filter
Activity a3 = {  0      , -1       , 2      , 2     , 0    , NULL  , NULL }; // mutate
Activity a4 = {  0      , -1       , 0      , 2     , 0    , NULL  , NULL }; // mutate
Activity a5 = {  0      , -1       , 3      , 4     , 0    , NULL  , NULL }; // insertafter
Activity a6 = {  0      , -1       , 1      , 9     , 0    , NULL  , NULL }; // unknown

Activity * trace[] = { &a0, &a1, &a2, &a3, &a4, &a5, &a6 };
int count = (sizeof trace / sizeof trace[0]);
int num_provided = 0;


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "sample-provider",
	.version = NULL,
	.intents = FEIGN_PROVIDER,
};


Layer layer_sample = {
	.name = "sample",
	.intents = FEIGN_PROVIDER,
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


	//feign_register_layer(&layer_sample);


	// announce features and request further action
	return &plugin;
}


Activity * destroy(Activity * activity) {
	CDEBUG("destroy");
	// As activities provided by this plugin are not dynamically allocated we do
	// not need to clean up anything except providing the destroy handler
	if ( activity->provider == plugin.instance_id )
		return NULL;
	else
		return activity;
}


Activity * provide(Activity * activity) {
	CDEBUG("provide");
	activity = NULL;
	if ( 0 <= num_provided && num_provided < count ) {
		activity = trace[num_provided];
		activity->provider = plugin.instance_id;
		num_provided++;
	}
	return activity;
}

Activity * reset(Activity * activity) {
	CDEBUG("reset");
	num_provided = 0;
}



/**
 * When the finalize hook has been registered, feign calls this to give the
 * plugin a chance to free resources and possibly dump results.
 *
// Activity:  {  status , provider , offset , layer , size , data* , stats* }
 * Could be only called when also registered OR should it be required?
 */
int finalize() {

	return 0;
}

