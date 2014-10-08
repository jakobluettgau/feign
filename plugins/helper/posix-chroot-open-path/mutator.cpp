#include <errno.h>
#include <fcntl.h> // for posix fadvise
#include <iostream>
#include <libgen.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <feign.h>
#include "posix/datatypes.h"



// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "fix-open-path",
	.version = NULL,
	//.intents = FEIGN_MUTATOR_CONTEXT | FEIGN_FILTER_CONTEXT | FEIGN_DESTROYER,
	.intents = FEIGN_MUTATOR,
};

int layer_id = 13;


// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello from %s!\n", plugin.name);

	// return plugin
	return &plugin;
}



Activity * mutate(Activity * activity) {
	FEIGN_LOG(1, "mutate");

	if ( activity->layer == layer_id ) 
	{
		posix_activity * sub_activity = (posix_activity*)activity->data;
		void * data = sub_activity->data;
		
		if ( sub_activity->type == POSIX_open && activity->status != 77 )
		{
			posix_open_data * d = (posix_open_data*) data;
			feign_log(1, "*open: pathname: %s, flags: %d, translatedFlags=%d, ret: %d\n", d->pathname, d->flags, d->translatedFlags, d->ret);

			const char * path = d->pathname;

			char *dirc, *basec, *bname, *dname;
			dirc = strdup(path);
			basec = strdup(path);
			dname = dirname(dirc);
			bname = basename(basec);
			printf("basename: %s\n", bname);
			d->pathname = feign_chroot_path(bname);
			printf("chrooted: %s\n", d->pathname );

			//free(d->pathname);

			feign_log(1, "+open: pathname: %s, flags: %d, translatedFlags=%d, ret: %d\n", d->pathname, d->flags, d->translatedFlags, d->ret);
			
		}
	}

	return activity;
}



int finalize() {

	return 0;
}

