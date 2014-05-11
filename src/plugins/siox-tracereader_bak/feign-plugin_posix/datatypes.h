#ifndef DATATYPES_H_E98GWOMP
#define DATATYPES_H_E98GWOMP

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Map functions to an integer for efficiency
 */
enum functions {
	POSIX_OPEN,
	POSIX_READ,
	POSIX_WRITE,
	POSIX_CLOSE,
	POSIX_DUP,
	POSIX_DUP2,
	POSIX_DUP3,
	POSIX_LSEEK,

	POSIX_FOPEN,
	POSIX_FREAD,
};

/**
 * As feign does only distinguish layers, different types within a layer could
 * be seperated by wrapping the data again.
 */
//@serializable
typedef struct posix_activity {
	int type;
	void * data;
	int ret;
} posix_activity;



// posix_(close|dup)
struct posix_fd_only_struct {
	int fd;
	int ret;
};
typedef struct fd_only_struct posix_close_data;


// posix_open*
struct posix_open_struct {
	const char * name;
	int flags;
	mode_t mode;
	int ret;
};
typedef struct posix_open_struct posix_open_data;
typedef struct posix_open_struct posix_open64_data;


// posix_creat*
struct posix_creat_struct {
	char * name;
	mode_t mode;
	int ret;
};
typedef struct posix_creat_struct posix_creat_data;
typedef struct posix_creat_struct posix_creat64_data;


// posix_(write|read)
struct posix_rw_struct {
	int fd;
	// ommited buf
	size_t count;
	int ret;
};
typedef struct posix_rw_struct posix_write_data;
typedef struct posix_rw_struct posix_read_data;


// posix_dup(|2|3)
struct posix_dup_struct {
	int oldfd;
	int ret;
};
typedef struct posix_dup_struct posix_dup_data;

struct posix_dup2_struct {
	int oldfd;
	int newfd;
	int ret;
};
typedef struct posix_dup2_struct posix_dup2_data;

struct posix_dup3_struct {
	int oldfd;
	int newfd;
	int flags;
	int ret;
};
typedef struct posix_dup3_struct posix_dup3_data;


// posix lseek
struct posix_lseek_struct {
	int fd;
	off_t pos;
	int origin;
	off_t ret;
};
typedef struct posix_lseek_struct posix_lseek_data;


/* Buffered I/O */

struct posix_fopen_struct {
	const char * path;
	const char * mode;
	int filehandle; // deviate from sig
	int ret;		// deviate from sig
};
typedef struct posix_fopen_struct posix_fopen_data;



struct posix_fread_struct {
	// ommited void * buf
	size_t size;
	size_t nr;
	// ommited FILE * stream;
	int filehandle; // deviate from sig
	int ret;
};
typedef struct posix_fread_struct posix_fread_data;



struct posix_fgets_struct {
	// ommited char * str;
	int size;
	// ommited FILE * stream;
	int filehandle; // deviate from sig
	int ret;
};
typedef struct posix_fgets_struct posix_fgets_data;


#endif /* end of include guard: DATATYPES_H_E98GWOMP */
