#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "plugins.h"


int plugin_ids = -1;


// debug helper: print_bits(sizeof(i), &i);
void print_bits(int s,void* p){int i,j;for(i=s-1;i>=0;i--)for(j=7;j>=0;j--)printf("%u",(*((unsigned char*)p+i)&(1<<j))>>j);puts("");}

// forward type declarations
class LayerManager;

// allowed plugin handles
typedef Plugin * (*init_handler)(void);
typedef Activity * (*activity_handler)(Activity *);
typedef int (*activity_context_handler)(std::list<Activity*>::iterator, std::list<Activity*>::iterator, std::list<Activity*>);

// shared object management
std::list<void*> dlopen_handles;
std::list<LayerManager*> layers;

/* Each layer.name will get it's own LayerManager wich keeps track of all the
 * providers, filters, mutators and replayers.
 */
class LayerManager
{
public:
	LayerManager (Layer * layer) {
		layer->name = name;
	};

private:

	char * name;
	std::list<activity_handler> providers;
	std::list<activity_handler> filters;
	std::list<activity_handler> mutators;
	std::list<activity_handler> replayers;
};


// TODO: manage with LayerManager
std::list<activity_handler> providers;
std::list<activity_handler> provider_resets;
std::list<activity_handler> filters;
std::list<activity_handler> mutators;

std::list<activity_context_handler> context_filters;
std::list<activity_context_handler> context_mutators;

std::list<activity_handler> precreators;
std::list<activity_handler> precreate_checkers;
std::list<activity_handler> replayers;
std::list<activity_handler> destroyers;

// plugin management interface
int plugin_manager_discover()
{
	return 0;
}


/**
 * @helper
 */
void dlsym_push_back_handler(void * handle, std::list<activity_handler> * list, char const * symbol, char const * errortext) {
	activity_handler handler;
	char *error;
	// TODO gain fault tolerence by trying multiple signatures?
	// e.g. Activity * handler(void) and Activity * handler(Activity * a)
	handler = (activity_handler)dlsym(handle, symbol);
	if ((error = dlerror()) != NULL) {
		fputs(error, stderr);
		fputs(errortext, stderr);
		exit(1);
	}
	(*list).push_back(handler);
}


void dlsym_push_back_context_handler(void * handle, std::list<activity_context_handler> * list, char const * symbol, char const * errortext) {
	activity_context_handler handler;
	char *error;
	// TODO gain fault tolerence by trying multiple signatures?
	// e.g. Activity * handler(void) and Activity * handler(Activity * a)
	handler = (activity_context_handler)dlsym(handle, symbol);
	if ((error = dlerror()) != NULL) {
		fputs(error, stderr);
		fputs(errortext, stderr);
		exit(1);
	}
	(*list).push_back(handler);
}


// Plugin Manager /////////////////////////////////////////////////////////////

/**
 * prints plugin manager statistics, e.g. number of plugins, etc..
 */
void plugin_manager_print_stats()
{
	printf("Plugin Statistics\n");
	printf("Providers:   %-5ld\n", providers.size() );
	printf("Filters:     %-5ld\n", filters.size() );
	printf("Mutators:    %-5ld\n", mutators.size() );
	printf("Precreators: %-5ld\n", precreators.size() );
	printf("Replayers:   %-5ld\n", replayers.size() );
	printf("Destroyers:  %-5ld\n", destroyers.size() );
	printf("\n");
}


/**
 * Try to load a shared object as a feign plugin, start registration ping pong.
 *
 * usage dlopen/dlclose will leak 1 block of memory once (not for every call)
 */
int pm_load_handlers(void * handle, int intents) {

	printf("Intent map: ");
	print_bits(sizeof(int), &intents);

	if ( FEIGN_UNDEFINED == intents ) {
		printf("'-> This plugin seems to do nothing. Specify some intents/features. (See: feign_plugin_intents)\n");
	}

	if ( FEIGN_PROVIDER & intents ) {
		printf("'-> This plugin wants to register as a provider.\n");
		dlsym_push_back_handler(handle, &providers, "provide",
			"\nMake sure to define \"Activity * provide(Activity * activity) {}\" when the FEIGN_PROVIDER flag is set.\n");
		dlsym_push_back_handler(handle, &destroyers, "destroy",
			"\nMake sure to define \"Activity * destroy(Activity * activity) {}\" when the FEIGN_PROVIDER flag is set.\n");
		// TODO: this is only a workarround, maybe unloading and reloading .so is better
		dlsym_push_back_handler(handle, &provider_resets, "reset",
			"\nMake sure to define \"Activity * reset(Activity * activity) {}\" when the FEIGN_PROVIDER flag is set.\n");
	}

	if ( FEIGN_FILTER & intents ) {
		printf("'-> This plugin wants to register as a filter.\n");
		dlsym_push_back_handler(handle, &filters, "filter",
			"\nMake sure to define \"Activity * filter(Activity * activity) {}\" when the FEIGN_FILTER flag is set.\n");
	}

	if ( FEIGN_MUTATOR & intents ) {
		printf("'-> This plugin wants to register as a mutator.\n");
		dlsym_push_back_handler(handle, &mutators, "mutate",
			"\nMake sure to define \"Activity * mutate(Activity * activity) {}\" when the FEIGN_MUTATOR flag is set.\n");
	}

	if ( FEIGN_FILTER_CONTEXT & intents ) {
		printf("'-> This plugin wants to register as a context aware filter.\n");
		dlsym_push_back_context_handler(handle, &context_filters, "filter_context",
			"\nMake sure to define \"Activity * filter_context(Activity * activity) {}\" when the FEIGN_FILTER_CONTEXT flag is set.\n");
	}

	if ( FEIGN_MUTATOR_CONTEXT & intents ) {
		printf("'-> This plugin wants to register as a context aware mutator.\n");
		dlsym_push_back_context_handler(handle, &context_mutators, "mutate_context",
			"\nMake sure to define \"Activity * mutate_context(Activity * activity) {}\" when the FEIGN_MUTATOR_CONTEXT flag is set.\n");
	}

	if ( FEIGN_REPLAYER & intents ) {
		printf("'-> This plugin wants to register as a replayer.\n");
		dlsym_push_back_handler(handle, &replayers, "replay",
			"\nMake sure to define \"Activity * replay(Activity * activity) {}\" when the FEIGN_REPLAYER flag is set.\n");
	}

	if ( FEIGN_PRECREATOR & intents ) {
		printf("'-> This plugin wants to register as a precreator.\n");
		dlsym_push_back_handler(handle, &precreators, "precreate",
			"\nMake sure to define \"Activity * precreate(Activity * activity) {}\" when the FEIGN_PRECREATOR flag is set.\n");
		dlsym_push_back_handler(handle, &precreate_checkers, "precreate_check",
			"\nMake sure to define \"Activity * precreate_check(Activity * activity) {}\" when the FEIGN_PRECREATOR flag is set.\n");
	}

	if ( FEIGN_DESTROYER & intents ) {
		printf("'-> This plugin wants to register as a destroyer.\n");
		dlsym_push_back_handler(handle, &destroyers, "destroy",
			"\nMake sure to define \"Activity * destroy(Activity * activity) {}\" when the FEIGN_PROVIDER flag is set.\n");
	}

	return 0;
}



int feign_register_layer(Layer * layer) {
	printf("feign_register_layer\n");
	print_bits(sizeof(int), &layer->intents);

	return 0;
}



int plugin_manager_load_plugin(char const * path) {
	return plugin_manager_load_plugin_mode(path, RTLD_LOCAL);
}

int plugin_manager_load_plugin_global(char const * path) {
	return plugin_manager_load_plugin_mode(path, RTLD_GLOBAL);
}

int plugin_manager_load_plugin_mode(char const * path, int mode)
{
	void *handle;
	char *error;

	init_handler init;

	handle = dlopen (path, RTLD_LAZY | mode);
	if (!handle) {
		fputs (dlerror(), stderr);
		exit(1);
	}

	init = (init_handler)dlsym(handle, "init");
	if ((error = dlerror()) != NULL) {
		fputs(error, stderr);
		exit(1);
	}

	// TODO plugin should receive identifies, it gains self addressing capabilities
	// e.g. the activity might be extend to require action


	Plugin * plugin = NULL;
	plugin = (*init)();

	printf("Plugin \"%s\" successfully loaded.\n", plugin->name);

	plugin_ids++;
	plugin->instance_id = plugin_ids;

	// load provide, filter, .., replay handles
	pm_load_handlers(handle, plugin->intents);

	printf("Plugin handle: %p\n\n", handle);
	dlopen_handles.push_back(handle);

	return 0;
}


/**
 * unload a plugin dlclosing the dlsym handle
 *
 * TODO: check for and call plugin_finalize
 */
int plugin_manager_unload_plugin(void * handle)
{


	// necessary
	printf("Unload plugin: %p\n", handle);
	dlclose(handle);
	return 0;
}


/**
 * unload all plugins by calling their unload functions
 */
int plugin_manager_unload_plugins()
{


	for (auto it=dlopen_handles.begin(); it != dlopen_handles.end(); ++it)
	{
		plugin_manager_unload_plugin(*it);
	}

	return 0;
}


// Activity Handlers //////////////////////////////////////////////////////////

/**
 * seek new activities from providers
 */
Activity * activity_provide() {
	Activity * activity = NULL;

	for (auto it=providers.begin(); it != providers.end(); ++it)
	{
		activity = (*it)(NULL);
		if ( activity != NULL )
			break;
	}

	return activity;

}

/**
 * pass activity to the provider and issue destroy
 */
 Activity * activity_filter(Activity * activity) {
	Activity * result;
	for (auto it=filters.begin(); it != filters.end(); ++it)
	{
		result = (*it)(activity);

		// NULL?, activtiy filtered!
		if ( result == NULL )
			return NULL;
	}
	return activity;
}

/**
 * pass activity to the provider and issue destroy
 */
Activity * activity_mutate(Activity * activity) {
	Activity * result;
	for (auto it=mutators.begin(); it != mutators.end(); ++it)
	{
		result = (*it)(activity);

		// NULL?, activtiy filtered!, however mutator is expected to free!
		if ( result == NULL )
			return NULL;

		// activity may have been exchanged swap
		activity = result;
	}
	return activity;
}

/**
 * pass iterator to context aware filter
 */
int activity_filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list)
{
	int ret;
	for (auto it=context_filters.begin(); it != context_filters.end(); ++it)
	{
		ret = (*it)(iter, end, list);

		// 1? activtiy filtered!
		if ( ret == 1 )
			return 1;
	}

	return 0;
}

/**
 * pass iterator to context aware mutator
 */
int activity_mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list)
{
	int ret;
	for (auto it=context_mutators.begin(); it != context_mutators.end(); ++it)
	{
		ret = (*it)(iter, end, list);

		// 1? activtiy filtered!
		if ( ret == 1 )
			return 1;
	}

	return 0;
}


/**
 * pass activity to replayers and issue replay
 */
int activity_precreate_check(Activity * activity) {
	Activity * result;
	for (auto it=precreate_checkers.begin(); it != precreate_checkers.end(); ++it)
	{
		result = (*it)(activity);
	}
	return 0;
}

/**
 * pass activity to replayers and issue replay
 */
int activity_precreate(Activity * activity) {
	Activity * result;
	for (auto it=precreators.begin(); it != precreators.end(); ++it)
	{
		result = (*it)(activity);
	}
	return 0;
}


/**
 * pass activity to replayers and issue replay
 */
int activity_replay(Activity * activity) {
	Activity * result;
	for (auto it=replayers.begin(); it != replayers.end(); ++it)
	{
		result = (*it)(activity);
	}
	return 0;
}

/**
 * pass activity to the provider and issue destroy
 */
int activity_destroy(Activity * activity) {
	Activity * result;
	for (auto it=destroyers.begin(); it != destroyers.end(); ++it)
	{
		result = (*it)(activity);
		if ( result == NULL )
			break;
	}
	return 0;
}


/**
 * pass activity to replayers and issue replay
 */
int activity_reset(Activity * activity) {
	Activity * result;
	for (auto it=provider_resets.begin(); it != provider_resets.end(); ++it)
	{
		result = (*it)(activity);
	}
	return 0;
}
