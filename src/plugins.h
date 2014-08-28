#ifndef PLUGINS_H_ENCMHGFO
#define PLUGINS_H_ENCMHGFO

#include <list>
#include "common.h"

// layer management
int layer_manager_register_layer();


// plugin management interface
void plugin_manager_print_stats();
int plugin_manager_discover();
int plugin_manager_load_plugin(char const * path);
int plugin_manager_load_plugin_global(char const * path);
int plugin_manager_load_plugin_mode(char const * path, int mode);
int plugin_manager_load_plugins(char const ** plugins);
int plugin_manager_unload_plugin(void * handle);
int plugin_manager_unload_plugins();


// TODO: info service proposel
int info_service(int instance_id);

// activity handling
Activity * activity_provide();
Activity * activity_filter(Activity * activity);
Activity * activity_mutate(Activity * activity);

int activity_filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list);
int activity_mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list);


int activity_precreate_check(Activity * activity);
int activity_precreate(Activity * activity);

int activity_replay(Activity * activity);
int activity_destroy(Activity * activity);

int activity_reset(Activity * activity);
int activity_report(Activity * activity);
#endif /* end of include guard: PLUGINS_H_ENCMHGFO */
