#include <stdio.h>

#include "common.h"
#include "options.h"


int register_env() {
	// TODO register arg from env impl
	return 0;
}

// parsing env
//TODO list of env options
void parse_env() {
	//TODO override options
}


//
// ARGUMENTS
//

// core args
static GOptionEntry arguments[] =
{
  // debug related
  {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Verbose mode", NULL },
  {"version", 'V', 0, G_OPTION_ARG_NONE, &version, "Display version information", NULL },
  {"loglevel", NULL, 0, G_OPTION_ARG_INT, &loglevel, "Set log level (higher = more).", NULL },
  
  // replay related 
  {"lookahead", NULL, 0, G_OPTION_ARG_INT, &lookahead, "Set fixed lookahead.", NULL },
  {"precreation", NULL, 0, G_OPTION_ARG_INT, &with_precreation, "Also perform a precreation run.", NULL },
  {"chroot", NULL, 0, G_OPTION_ARG_FILENAME_ARRAY, &option_chroot, "Set chroot to be used by plugins (if supported by plugin)", "./chroot" },

  // plugin loading
  {"plugin",  'p', 0, G_OPTION_ARG_FILENAME_ARRAY, &plugins, "Load plugin", "plugin.so" },
  {"plugin-global",  'P', 0, G_OPTION_ARG_FILENAME_ARRAY, &plugins_global, "Load plugin with RTLD_GLOBAL", "plugin.so" },

  // placeholders (list shall terminate with NULL)
  { NULL },
  { NULL },
  { NULL },
};

int register_arg() {
	// TODO implement register arg
	return 0;
}

/**
 * Glib argument parsing encapsulated to easily swap implementations if needed.
 * argc is passed as a pointer so argc can be updated, as glib removes parsed
 * arguments after interpretation.
 */
void parse_args(int * argc, char *argv[])
{
	GError *error = NULL;
	GOptionContext *context;

	context = g_option_context_new ("- flexible event imitation engine, to replay trace files");
	g_option_context_add_main_entries (context, arguments, NULL);
	g_option_context_set_ignore_unknown_options(context, TRUE);
	if (!g_option_context_parse (context, argc, &argv, &error))
	{
		g_print ("option parsing failed: %s\n", error->message);
		//exit (1);
	}

}

void print_args(int argc, char *argv[])
{
	int i;
	if ( argc > 1 ) {
		for (i = 0; i < argc; ++i)
		{
			printf("argv[%d] = %s\n", i, argv[i]);
		}
	}
}
