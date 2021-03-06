#ifndef FEIGN_PLUGIN_H_AYZ49SYK
#define FEIGN_PLUGIN_H_AYZ49SYK

#ifdef FEIGN_NO_CPP_INCLUDES
// do not include/declare/define
// provided to work around namespace issues
#else
#include <list>
#endif


//#define DEBUG(a) printf("%-30s %3d %s:%d\n", a, plugin.instance_id, __FILE__, __LINE__)
#define DEBUG(a)
//#define CDEBUG(a) printf("%-30s %3d %s:%d\n", a, plugin.instance_id, __FILE__, __LINE__)
#define CDEBUG(a)







/**
 * To label activities, according to their status within feigns pipeline.
 */
enum feign_activity_info {
	FEIGN_STATUS_NEW,
	FEIGN_STATUS_PREPROCESSED,
	FEIGN_STATUS_MUTATED,
	FEIGN_STATUS_READY,
	FEIGN_STATUS_REPLAYED,
	FEIGN_STATUS_DROPPED,
	FEIGN_STATUS_DATA_FREED,
	FEIGN_STATUS_RECYCLE,
};

/**
 * Statistic for an activity
 */
//@serializable
typedef struct Statistic {
	long start;
	long end;
	int status;
} Statistic;



/**
 * Intermediate Datatype: FunctionArgument, to get rid of void* for data
 */
//@serializable
typedef union FunctionArgumentValue {
	char * c;
	int * i;
	long * l;
	float * f;
	double * d;
} FunctionArgumentValue;

//@serializable
typedef struct FunctionArgument {
	int type;
	FunctionArgumentValue value;
} FunctionArgument;


/**
 * The activity datatype that is known to all plugins, it is the communication
 * backbone for everything replay sepcific.
 * It holds only what is absolutely necessary to allow "scheduling" and event
 * for replay and to build context sensitive filters and mutators. It is however
 * flexiable to an extend that plugins can attach data. Feign makes no attempt
 * to change the data pointed to, but needs to know the size for serialisation.
 */
//@serializable
typedef struct Activity {
	int status;  // like to discard an activity without reorganisation penalty
	int provider;
	long long offset;
	int layer;
	int size;
	void * data;
	Statistic * stats;
} Activity;



/**
 * Features and intents a plugin can announce. These information are gathered
 * to allow for an more optimized datahandling.
 */
enum feign_plugin_intents {
	FEIGN_UNDEFINED            = 0,
	FEIGN_PROVIDER             = (1 << 0),
	FEIGN_FILTER               = (1 << 1),
	FEIGN_FILTER_CONTEXT       = (1 << 2),  // or LOOKAHEAD?
	FEIGN_MUTATOR              = (1 << 3),
	FEIGN_MUTATOR_CONTEXT      = (1 << 4),
	FEIGN_WRITER               = (1 << 5),
	FEIGN_REPLAYER             = (1 << 6),
//	FEIGN_REPLAYER_EXCLUSIVE   = (1 << 7),
//	FEIGN_REPLAYER_PSEUDO      = (1 << 8),
	FEIGN_PRECREATOR           = (1 << 9),
	FEIGN_PROVIDE_HOOK         = (1 << 10),
	FEIGN_REQUIRE_HOOK         = (1 << 11),
	FEIGN_FILTER_CONTEXT_FREE  = (1 << 12),
	FEIGN_MUTATOR_CONTEXT_FREE = (1 << 13),
};

/**
 * Provided by a plugin during registration to introduce itself to feign. The
 * name and version strings are used to identify the plugin and satisfy possible
 * plugin dependencies. Based on the num_layers field, the plugin can announce
 * the number of layers it wants to register for.
 */
typedef struct Plugin {
	const char * name;
	char * version;
	int intents;
	int instance_id;
	void * dlopen;
} Plugin;

/**
 * Provided during layer registration. The plugin provides the layer name, and
 * specifies its capabilities and intents concerning the layer (e.g. provide,
 * filter, mutate or replay, pseudo-replay).
 *
 * The activity.type is determined during runtime and provides and efficent
 * way to store preprocessed tracedata for a given plugin configuration.
 */
typedef struct Layer {
	const char * name;
	int intents;
	int * layer_id; // determined during runtime
} Layer;




/**
 * Plugin and Layer registration and info service APIs
 */



extern "C" {

/**
 * plugins may register to receive arguments or environment variables
 *
 */
//int feign_register_arg(GOptionEntry arg);
int feign_register_arg(const char * arg);
int feign_register_env(const char * env);

/**
 * Feign maintains a layer map to allow for optimisation and to allow
 * plugins to easily hook onto the layer procession path
 *
 * @param	layer	layer struct with name and intends
 */
int feign_register_layer(Layer * layer);

int feign_update_lookahead(int newlookahead);

int feign_register_hook_callback(char * hook, void * callback); // feign_callback_type?
int feign_register_hook_dlsym(char * hook, char * symbol);

void feign_log(char const * message, int log_level);

char * feign_shared_buffer(int bytes);
int feign_precreate_file(const char * filename, size_t size);

}

/**
 * Prevent the plugin handlers from beeing mangled. This hopefully helps to
 * keep plugin source short and simple.
 */
extern "C" {
	Plugin * init();
	int finalize();
	Activity * provide(Activity * activity);
	Activity * filter(Activity * activity);
	Activity * mutate(Activity * activity);
#ifdef FEIGN_NO_CPP_INCLUDES
// do not include/declare/define
// provided to work around namespace issues
#else
	int filter_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list);
	int mutate_context(std::list<Activity*>::iterator iter, std::list<Activity*>::iterator end, std::list<Activity*> list);
#endif
	Activity * precreate_check(Activity * activity);
	Activity * precreate(Activity * activity);
	Activity * replay(Activity * activity);
	Activity * destroy(Activity * activity);
	Activity * reset(Activity * activity);
}

#endif /* end of include guard: FEIGN_PLUGIN_H_AYZ49SYK */

