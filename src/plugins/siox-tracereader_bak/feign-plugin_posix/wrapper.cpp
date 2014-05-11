#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <map>

#include "wrapper.h"

//#define DEBUG(a) printf("%-30s %3d %s:%d\n", a, -1, __FILE__, __LINE__)
#define DEBUG(a)



/**
 * Map the trace filehandles to actual file handles
 */
int issued = 3;	// 3 will be first issued by this replayer

std::map<int,int> fds;
std::map<int,FILE> streams;

void dump_map() {
	printf("dump_map()\n");
	for (auto it=fds.begin(); it!=fds.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
	}
}


/**
 * The actual wrappers
 */
int posix_open(void * data) {
	posix_open_data * d = (posix_open_data*) data;
	printf("d: name=%s, flags=%d, mode=%d, ret=%d\n", d->name, d->flags, d->mode, d->ret);

	// get filehandle
	int fd = issued;
	issued++;



	// add to map
	fds[d->ret] = fd;

	DEBUG("'-open()");
	return 0;
}

int posix_read(void * data) {
	posix_read_data * d = (posix_read_data*) data;
	DEBUG("'-read()");
	return 0;
}

int posix_write(void * data) {
	posix_write_data * d = (posix_write_data*) data;
	DEBUG("'-write()");
	return 0;
}

int posix_close(void * data) {
	//posix_close_data * d = (posix_close_data*) data;
	DEBUG("'-close()");
	return 0;
}


int posix_fclose(void * data) {
	//posix_fclose_data * d = (posix_fclose_data*) data;
	DEBUG("'-fclose()");
	return 0;
}


int posix_lseek(void * data) {
	posix_lseek_data * d = (posix_lseek_data*) data;
	DEBUG("'-lseek()");
	return 0;
}




/**
 * Buffered I/O
 */
int posix_fopen(void * data) {
	//posix_open_data * d = (posix_open_data*) data;
	//printf("d: name=%s, flags=%d, mode=%d, ret=%d\n", d->name, d->flags, d->mode, d->ret);

	// get filehandle
	//int fd = issued;
	//issued++;



	// add to map
	//fds[d->ret] = fd;

	DEBUG("'-fopen()");
	return 0;
}

int posix_fread(void * data) {
	//posix_read_data * d = (posix_read_data*) data;
	DEBUG("'-fread()");
	return 0;
}
