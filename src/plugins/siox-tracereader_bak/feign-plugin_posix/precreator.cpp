#include <stdio.h>
#include <iostream>
#include <map>
#include <string>

// include feign helpers for easier plugin development
//////////////////////////////////////////////////////
#include <feign.h>

// layer includes
/////////////////
#include "datatypes.h"
#include "wrapper.h"


// precreation related data
///////////////////////////

/**
 * Specification:
 *
 * We will ignore the standard filehandles (0=stdin, 1=stdout, 2=stderr),
 * feign will print things if the trace includes them, so maybe it should
 * be offered by argument to write them elsewhere.
 *
 * offset + byte > filesize so far require to enlarge the file
 */
struct fileinfo
{
	std::string filepath;	// path to file, not really needed for our purpose
	int create_size;		// the minium file size when precreated
	int size;				// running file size
	int flags;				// file open flags r? r+? w? rw? a?
	int mode;				// file permissions, is ignored here
	int pos;				// file handle position
	int precreate;			// flag that this needs to be precreated
};

// map filepath to handle
std::map<int, fileinfo> fd2info;		    // sizes not populated, lookup in global
std::map<std::string, fileinfo> file2ginfo; // global fileinfo, keeps track of size, mode and flags not cared for



void dump_fd2info() {
	printf("dump_fd2info)\n");
	for (auto it=fd2info.begin(); it!=fd2info.end(); ++it) {
		std::cout << it->first << ".filepath => " << it->second.filepath << '\n';
		std::cout << it->first << ".size => " << it->second.size << '\n';
		std::cout << it->first << ".create_size => " << it->second.create_size << '\n';
		std::cout << it->first << ".flags => " << it->second.flags << '\n';
		std::cout << it->first << ".mode => " << it->second.mode << '\n';
		std::cout << it->first << ".pos => " << it->second.pos << '\n';
		std::cout << it->first << ".precreate => " << it->second.precreate << '\n';
	}
}

void dump_file2ginfo() {
	printf("dump_file2ginfo)\n");
	for (auto it=file2ginfo.begin(); it!=file2ginfo.end(); ++it) {
		std::cout << it->first << ".filepath => " << it->second.filepath << '\n';
		std::cout << it->first << ".size => " << it->second.size << '\n';
		std::cout << it->first << ".create_size => " << it->second.create_size << '\n';
		std::cout << it->first << ".flags => " << it->second.flags << '\n';
		std::cout << it->first << ".mode => " << it->second.mode << '\n';
		std::cout << it->first << ".pos => " << it->second.pos << '\n';
		std::cout << it->first << ".precreate => " << it->second.precreate << '\n';
	}
}


int layer_id = 13;


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "posix-precreator",
	.version = NULL,
	.intents = FEIGN_PRECREATOR,
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
Activity * precreate_check(Activity * activity) {
	DEBUG("precreate_check");

	if ( activity->layer == layer_id ) {
		posix_activity * pactivity = (posix_activity*)activity->data;

		// a simple way to produce action
		switch ( pactivity->type ) {

			case POSIX_OPEN:
				{
					posix_open_data * d = (posix_open_data*) pactivity->data;
					printf("d: name=%s, flags=%d, mode=%d, ret=%d\n", d->name, d->flags, d->mode, d->ret);
					DEBUG("open()");


					if ( fd2info.count(d->ret) )  {
						// trace invalid
						DEBUG("Invalid tracedata, file handle collision!");
					} else {
						DEBUG("Add to precreate!!");

						fileinfo fi = { .filepath = d->name };
						fi.precreate = 1;
						fd2info[d->ret] = fi;
						file2ginfo[d->name] = fi;

					}

					break;
				}

			case POSIX_READ:
				{
					posix_read_data * d = (posix_read_data*) pactivity->data;
					DEBUG("read()");
					break;
				}

			case POSIX_WRITE:
				{
					posix_write_data * d = (posix_write_data*) pactivity->data;
					DEBUG("write()");
					break;
				}

			case POSIX_CLOSE:
				{
					posix_close_data * d = (posix_close_data*) pactivity->data;
					DEBUG("close()");
					break;
				}

			default: {
				DEBUG("unknown type");
			}
		}
	} else {
		DEBUG("!");
	}

	dump_fd2info();
	dump_file2ginfo();

	return NULL;
}




Activity * precreate(Activity * activity) {
	DEBUG("precreate");

	for (auto it=file2ginfo.begin(); it!=file2ginfo.end(); ++it) {

		if ( it->second.precreate ) {
			std::cout << it->first << ".filepath => " << it->second.create_size << '\n';
		}
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

