#include <stdio.h>
#include <list>

#include <feign.h>
#include <posix/datatypes.g>

// some data
////////////


// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "sample-mutator-context",
	.version = NULL,
	.intents = FEIGN_MUTATOR_CONTEXT,
};

// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello sample plugin!\n");

	// return plugin
	return &plugin;
}


int mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	CDEBUG("mutate_context");

	Activity * activity = (*iter);

	int check_num = 5;

	int last = NULL;


	while ( iter != end && check_num > 0 )
	{
		printf("m: activity->layer = %d, check_num = %d\n", activity->layer, check_num);

		last = activity->layer;

		iter++;
		/*
		activity = (*iter);
		// insert after 4 if not followed by 5
		if ( last == 4 && activity->layer != 5 ) {
			list.insert(iter, &a0);
		}
		*/

		check_num--;
	}


	//activity = (*it);
	//printf("f: activity->layer = %d\n", activity->layer);

	/*
	if ( activity->layer == 1 )
		return NULL;
	else
		return activity;
	*/

	return 0;
}


int finalize() {

	return 0;
}

