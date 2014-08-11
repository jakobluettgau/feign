#ifndef DATATYPES_H_R58DWT45F
#define DATATYPES_H_R58DWT45F

/**
 * As feign does only distinguish layers, different types within a layer could
 * be seperated by wrapping the data again.
 */
//@serializable
typedef struct posix_activity {
	int type;
	void * data;
	void * siox_activity;
	int ret;
} posix_activity;

#endif // DATATYPES_H_R58DWT45F
