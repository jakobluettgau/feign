#ifndef COMMON_H_RE8NAI9Y
#define COMMON_H_RE8NAI9Y

#include <glib.h>

#include <feign.h>

// core options
// TODO the following variables don't get global scope, but every *.cpp has their own :/
extern gboolean verbose;			// display debug?
extern gboolean version;			// display feign version
extern gchar ** plugins;			// for normal shared libraries
extern gchar ** plugins_global;		// for shared libraries that require RTLD_GLOBAL

extern gboolean direct;				// do no preprocession runs, default: FALSE
extern gchar * precreation_path; 	// default to feign-<timestamp> ?
extern gchar * environment_path;	// TODO: propose feign tmp directory structure
extern gboolean rollback;			// rollback environment after replay?
extern int lookahead;			// rollback environment after replay?

extern int loglevel;

// debug, verbose and other helpers that are available throughout feign core
//void Debug();
//void Verbose();

#endif /* end of include guard: COMMON_H_RE8NAI9Y */
