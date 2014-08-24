#include <stdio.h>
#include <stdarg.h>

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <feign.h>

#include "helper.h"

extern int loglevel;

// General helpers for debugging and information
///////////////////////////////////////////////////////////////////////////////
void feign_log(int level, const char* format, ...)
{
	if ( level <= loglevel ) {
		va_list args;
		va_start(args,format);
		vprintf(format,args);
		va_end(args);
	}
}


/**
 * Create direcory structure and.
 */
void feign_chroot_setup() {

}


/**
 * Return chroot path that should be used by plugins.
 * 
 * TODO: establish environment information facility
 *
 * @return		string path
 */
const char* feign_chroot() {
	static char * chroot = "./feign-chroot-data";
	return chroot;
}



// I/O related helpers
///////////////////////////////////////////////////////////////////////////////
/**
 * Use the feign_shared_buffer() function  if dummy data is needed, to prevent
 * redundant dummy data in memory.
 *
 * This will keep a static buffer around, and reallocates more bytes as needed.
 */
char * feign_shared_buffer(unsigned int size){
	static char * buffer = NULL;
	static size_t buffer_size = 0;

	if (buffer_size >= size){
		return buffer;
	}

	// allocate memory for data to write
	if ((buffer = (char *)realloc(buffer, size))) {
		size_t i;
		for (i=buffer_size; i < size; i++){
			buffer[i] = '0';
		}

		buffer_size = size;

		feign_log(6, "Memory reallocated for %lld bytes\n", (long long int) size);
		return buffer;
	}
	else {
		feign_log(6, "Not enough memory available to allocate %lld bytes!\n",  (long long int) size);
		exit(1);
	}
}


int feign_precreate_file(const char * filename, size_t size) {
	int fd;
	int ret;
	ssize_t nr;

	size_t buf_size = 1024*4;

	char * buf = feign_shared_buffer(buf_size);

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);

	ret = posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
	if ( ret == -1 )
		perror("posix_fadvise");

	if ( fd != -1 ) {
		while ( size > 0 ) {
			if ( size < buf_size ) {
				nr = write(fd, buf, size);
				if (nr == -1)
					perror("write");
				size = 0;
			} else {
				nr = write(fd, buf, buf_size);
				if (nr == -1)
					perror("write");
				size -= buf_size;
			}
		}
		close(fd);
	} else {
		perror("open");
	}

	return 0;
}





// time related ///////////////////////////////////////////////////////////////

/**
 * start a timer and store to specified struct
 *
 * @param	start	pointer where start should be stored
 */
void start_timer(struct timespec * start) {
	int ret;

	ret = clock_gettime (CLOCK_MONOTONIC, start);
	if (ret)
		perror ("clock_gettime");
	//else {
	//	printf ("clock=%d sec=%ld nsec=%ld\n", CLOCK_REALTIME, ts->tv_sec, ts->tv_nsec);
	//}
}

/**
 * stop timer and print some statistics
 *
 * @param	start	pointer where start time is stored
 * @param	end		pointer where end time should be stored
 * @param	ops	-1 to ommit stats, number of ops completed between start and end
 */
void stop_timer(struct timespec * start, struct timespec * end, int ops) {
	int ret;

	ret = clock_gettime (CLOCK_MONOTONIC, end);
	if (ret)
		perror("clock_gettime");
	else {
		printf("start: clock=%d sec=%ld nsec=%ld\n", CLOCK_MONOTONIC, start->tv_sec, start->tv_nsec);
		printf("end:   clock=%d sec=%ld nsec=%ld\n", CLOCK_MONOTONIC, end->tv_sec, end->tv_nsec);

		struct timespec delta;

		// TODO seems fixed, watch
		if ( (end->tv_nsec - start->tv_nsec) < 0 ) {
			delta.tv_sec = end->tv_sec - start->tv_sec-1;
			delta.tv_nsec = 1000*1000*1000 - start->tv_nsec + end->tv_nsec;
		} else {
			delta.tv_sec = end->tv_sec - start->tv_sec;
			delta.tv_nsec = end->tv_nsec -  start->tv_nsec;
		}

		long double ddelta = delta.tv_sec*1000*1000 + (double)(delta.tv_nsec)/(1000);

		printf("delta: clock=%d sec=%ld nsec=%ld\n", CLOCK_MONOTONIC, delta.tv_sec, delta.tv_nsec);
		if ( ops != -1 ) {
			printf("us/cycle: %Lf\n", ddelta/ops );
			printf("ops/us: %Lf\n", ops/ddelta );


			printf("csv:");
			printf("%Lf,", ddelta);
			printf("%Lf,", ddelta/ops );
			printf("%Lf", ops/ddelta );
			printf("\n");

		}
		printf("\n");
	}

}

/**
 * for subsecond nanosleep, simple, will repeat sleep until target reached
 *
 * @param	ns	nanoseconds to sleep
 */
void simple_nanosleep(long ns) {
	int ret;

	struct timespec req = {
		.tv_sec = 0,
		.tv_nsec = ns
	};

	struct timespec rem, *a = &req, *b = &rem;

	while ( nanosleep(a,b) && errno == EINTR) {
		struct timespec *tmp = a;
		a = b;
		b = tmp;
	}
}


