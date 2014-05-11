#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>

#include "../TraceReader.hpp"

namespace feign {
	#define FEIGN_NO_CPP_INCLUDES
	#include "feign.h"
	#include "feign-plugin_posix/datatypes.h"
}

int main( int argc, char ** argv )
{
	int count = 0;

	TraceReader tr = TraceReader( "activities.dat", "system-info.dat", "ontology.dat", "association.dat" );
	//TraceReader tr = TraceReader( "lab/activities.dat", "lab/system-info.dat", "lab/ontology.dat", "lab/association.dat" );
	while( true ) {
		monitoring::Activity * a = tr.nextActivity();
		if( a == nullptr )
			break;

		count++;
		tr.printActivity( a );
		delete( a );
	}

	cout << "Count: " << count << "\n";

	return 0;
}


namespace feign {

	Plugin plugin = {
		.name = "siox-tracereader_posix-provider",
		.version = NULL,
		.intents = FEIGN_PROVIDER,
	};


	TraceReader * tr2;

	char const * get_dat_env(char const * name, char const * fallback) {
		char const * value;
		value = getenv (name);
		if (! value) {
			value = fallback;
		}
		else {
			printf ("%s = %s\n", name, value);
		}

		return value;
	}

	void create_trace_reader() {
		char const * activities = get_dat_env("FEIGN_SIOX_ACTIVITIES", "activities.dat");
		char const * systeminfo = get_dat_env("FEIGN_SIOX_SYSTEMINFO", "systeminfo.dat");
		char const * ontology = get_dat_env("FEIGN_SIOX_ONTOLOGY", "ontology.dat");
		char const * association = get_dat_env("FEIGN_SIOX_ASSOCIATION", "association.dat");

		if ( tr2 != NULL )
			delete tr2;

		tr2 = new TraceReader( (string)activities, (string)systeminfo, (string)ontology, (string)association );
	}


	Plugin * init() {
		// maybe provide an ENUM Bitmap Type to easilly announce capabilities
		// e.g. FEIGN_PLUGIN_SUCCESS && FEIGN_PLUGIN_PROVIDER && FEIGN_PLUGIN_REPLAYER
		// to say: init was completed succesfully, i can provide and replay tracedata
		// please call the appropiate handlers

		printf("Hello from %s\n", __FILE__);
		// gonna be provider and replayer
		// but provider should negotiate identifier

		//main(0, NULL);



		create_trace_reader();

		// announce features and request further action
		return &plugin;
	}





	Activity * create_activity(long offset, int type) {
		int layer_id = 13;
		Activity * activity = (Activity *) malloc(sizeof(Activity));

		posix_activity * sub_activity = (posix_activity *) malloc(sizeof(posix_activity));


		void * data;

		switch ( type ) {
			case POSIX_READ:
				DEBUG("create read()");
				break;

			case POSIX_FREAD:
				DEBUG("create fread()");
				break;

			case POSIX_WRITE:
				DEBUG("creat write()");
				break;

			case POSIX_OPEN:
				DEBUG("create open()");
				break;

			case POSIX_CLOSE:
				DEBUG("create close()");
				break;
		}




		sub_activity->type = type;
		//sub_activity->data = data;

		activity->layer = layer_id;
		activity->offset = offset;
		activity->data = sub_activity;
		activity->provider = plugin.instance_id;

		return activity;
	}

	Activity * convert_siox_to_feign(monitoring::Activity * a) {
		Activity * activity = NULL;


		std::string str_open ("open");
		std::string str_close ("close");
		std::string str_read ("read");
		std::string str_write ("write");
		std::string str_dup ("dup");
		std::string str_dup2 ("dup2");
		std::string str_dup3 ("dup3");
		std::string str_fread ("fread");
		std::string str_fopen ("fopen");

		string activity_name = tr2->s->lookup_activity_name( a->ucaid() );


		// write
		if ( str_write.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_WRITE);
		} else

		// read
		if ( str_read.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_READ);
		} else

		// dup, dup2, dup3
		if ( str_dup.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_DUP);
		} else

		if ( str_dup2.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_DUP2);
		} else

		if ( str_dup3.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_DUP3);
		} else

		// fclose, fopen
		if ( str_fopen.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_FOPEN);
		} else

		if ( str_fread.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_FREAD);
		} else

		// close, open
		if ( str_close.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_CLOSE);
		} else

		if ( str_open.compare(activity_name) == 0) {
			activity = create_activity(0, POSIX_OPEN);
		}


		// siox timestamps come in nanosecond resolution
		long offset = a->time_stop() - a->time_start();

		if ( activity != NULL ) {
			activity->offset = offset;

			//tr2->printActivity( a );
		}



		return activity;
	}


	Activity * provide(Activity * activity) {
		CDEBUG("provide");

		activity = NULL;

jump:

		monitoring::Activity * a = tr2->nextActivity();
		if( a == nullptr )
			return NULL;

		//tr2->printActivity( a );

		activity = convert_siox_to_feign(a);
		delete(a);

		if ( activity == NULL )
			goto jump;


		return activity;
	}


	void free_sub_activity(posix_activity * pa) {
		switch ( pa->type ) {
			case POSIX_READ:
				DEBUG("free read()");
				break;

			case POSIX_WRITE:
				DEBUG("free write()");
				break;

			case POSIX_OPEN:
				DEBUG("free open()");
				break;

			case POSIX_CLOSE:
				DEBUG("free close()");
				break;
		}

	}



	Activity * destroy(Activity * activity) {
		CDEBUG("destroy");

		if ( activity->provider == plugin.instance_id ) {
			if ( activity->data != NULL ) {
				posix_activity * sub_activity = (posix_activity *)(activity->data);

				if ( sub_activity->data != NULL ) {
					//free(sub_activity->data);
				} else {
				}
				free(sub_activity);
			} else {
			}
			free(activity);
			return NULL;
		} else {
			return activity;
		}

		return NULL;
	}



	Activity * reset(Activity * activity) {
		CDEBUG("reset");

		create_trace_reader();

		return NULL;
	}



}
