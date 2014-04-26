#include <stdio.h>
#include <list>

#include <feign.h>

// some data
////////////
// Activity:  {  status , provider , offset , layer , size , data* , stats* }
Activity a0 = {  0      , -1       , 1      , 5     , 0    , NULL  , NULL }; // sample



// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "sample-filter-context",
	.version = NULL,
	.intents = FEIGN_FILTER_CONTEXT,
};

// implement handlers
/////////////////////
Plugin * init() {
	// do some initialisation
	printf("Hello sample plugin!\n");

	// return plugin
	return &plugin;
}


int filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list) {
	CDEBUG("filter_context");

	Activity * activity = (*iter);

	int check_num = 2;


	while ( iter != end && check_num > 0 )
	{
		printf("f: activity->layer = %d\n", activity->layer);

		iter++;
		check_num--;
		activity = (*iter);

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

