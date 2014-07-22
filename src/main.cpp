/** @file */
#include <iostream>
#include <stdio.h>

#include <errno.h>
#include <time.h>

#include "common.h"			// currently includes glib
#include "options.h"
#include "plugins.h"
#include "replay.h"
#include "helper.h"

// set some default options
// TODO: reconsider dropping "real" globals within feign core
gboolean verbose = FALSE;
gboolean version = FALSE;
gchar ** plugins;
gchar ** plugins_global;
int lookahead;


int load_plugins(gchar ** list, int(*load_plugin)(char const *)) {
	int plugin_count = 0;
	if( list != NULL ) {
		char * plugin = NULL;
		int i = 0;
		while ( (plugin = plugins[i++]) ) {
			plugin_count++;
			load_plugin(plugin);
		}
	}
	printf("Option: count(plugin) = %d\n", plugin_count);

	return plugin_count;
}


int main(int argc, char *argv[])
{

	// in this order so that:
	// env overwrites application defaults
	// arg overwrites environment and/or application defaults
	parse_env();
	//print_args(argc, argv);
	parse_args(&argc, argv);
	//print_args(argc, argv);


	// DEBUG: listing options
	printf("feign running with options:\n");
	printf("Option: verbose = %d\n", verbose);
	printf("Option: version = %d\n", version);
	printf("Option: lookahead = %d\n", lookahead);

	// keep everything nice and tidy
	printf("\n");

	
	int num_plugins_loaded = 0;

	// load plugins from arguments
	num_plugins_loaded += load_plugins(plugins,        plugin_manager_load_plugin);
	num_plugins_loaded += load_plugins(plugins_global, plugin_manager_load_plugin_global);

	if ( num_plugins_loaded <= 0 ) {
		printf("\nThere is nothing to do!\n");
		return 0;
	}


	if ( lookahead < 1 ) {
		lookahead = 1;
	}


	// replay trace
	//
	//
	// phased replay
	//
	//

		int r = -1;
		int p = -1;


		printf("begin precreation ================================================\n");

		p = preload();
		while ( (r = replay()) != 1 ) {
			//printf("r: %d, p: %d\n", r, p);
			p = preload();
		}

		printf("precreation create files =========================================\n\n");

		activity_precreate(NULL);

		printf("finish precreation ===============================================\n\n");

		// reset
		replay_manager_print_stats();
		reset_replay_stats();
		reset_buffer_flags();
		activity_reset(NULL);

	struct timespec start;
	struct timespec end;



	// phased replay
	printf("begin replay =====================================================\n");
	start_timer(&start);
	r = -1;
	p = -1;
	p = preload();
	while ( (r = replay()) != 1 ) {
		//printf("r: %d, p: %d\n", r, p);
		p = preload();
	}
	stop_timer(&start, &end, 1e4);
	printf("finish replay ====================================================\n\n");

	replay_manager_print_stats();
	plugin_manager_print_stats();

	// finalize
	plugin_manager_unload_plugins();


	return 0;
}



