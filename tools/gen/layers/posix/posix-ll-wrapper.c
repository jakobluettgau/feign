#define _GNU_SOURCE
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h> // for canonicalize_file_name and realpath
#define HAVE_AIO
#ifdef HAVE_AIO
#include <aio.h>
#endif
#include <sys/sendfile.h>
#ifdef HAVE_AIO
#endif
#include <sched.h>
#include <sys/file.h>
#define INSTRUMENT_POSIX_COMMUNICATION
#ifdef INSTRUMENT_POSIX_COMMUNICATION
#include <sys/types.h>
#include <sys/socket.h>
#endif
// global includes: 
#include  <stdlib.h>
#include  <stdio.h>
#include  <stdarg.h>
#include  <glib.h>
#include  <C/siox.h>
#include  <assert.h>
#include  <string.h>


static siox_component * global_component = NULL;
      static siox_unique_interface * global_uid = NULL;
      static int global_layer_initialized = FALSE;
static siox_component * network_component = NULL;
      static siox_unique_interface * network_uid = NULL;
      static int network_layer_initialized = FALSE;
static siox_attribute * bytesToRead;
static siox_attribute * bytesToWrite;
static siox_attribute * filePosition;
static siox_attribute * fileExtent;
static siox_attribute * fileMemoryRegions;
static siox_attribute * fileOpenFlags;
static siox_attribute * fileName;
static siox_attribute * fileSystem;
static siox_attribute * fileHandle;
static siox_attribute * bytesWritten;
static siox_attribute * bytesRead;
static siox_attribute * fileAdviseExtent;
static siox_attribute * fileAdvise;
static siox_attribute * fileBufferSize;
static siox_attribute * fileBufferMode;
static GHashTable * activityHashTable_int;
GRWLock lock_activityHashTable_int;
static GHashTable * activityHashTable_network_int;
GRWLock lock_activityHashTable_network_int;
static GHashTable * activityHashTable_size; GRWLock lock_activityHashTable_size;
#define SET_FILENAME(ARG) char fileNamebuffer[PATH_MAX]; char * resPath = realpath(ARG, fileNamebuffer); if ( resPath == NULL ) siox_activity_set_attribute( sioxActivity, fileName, ARG ); else siox_activity_set_attribute( sioxActivity, fileName, fileNamebuffer );
#include "posix-low-level-io-helper.h"
static siox_component_activity * cvopen;
static siox_component_activity * cvcreat;
static siox_component_activity * cvopen64;
static siox_component_activity * cvcreat64;
static siox_component_activity * close_global;
				static siox_component_activity * close_network;
static siox_component_activity * dup_global;
				static siox_component_activity * dup_network;
static siox_component_activity * dup2_global;
				static siox_component_activity * dup2_network;
static siox_component_activity * dup3_global;
				static siox_component_activity * dup3_network;
static siox_component_activity * cvsendfile;
static siox_component_activity * write_global;
				static siox_component_activity * write_network;
static siox_component_activity * read_global;
				static siox_component_activity * read_network;
static siox_component_activity * writev_global;
				static siox_component_activity * writev_network;
static siox_component_activity * readv_global;
				static siox_component_activity * readv_network;
static siox_component_activity * pwrite_global;
				static siox_component_activity * pwrite_network;
static siox_component_activity * pread_global;
				static siox_component_activity * pread_network;
static siox_component_activity * pwrite64_global;
				static siox_component_activity * pwrite64_network;
static siox_component_activity * pread64_global;
				static siox_component_activity * pread64_network;
static siox_component_activity * pwritev_global;
				static siox_component_activity * pwritev_network;
static siox_component_activity * preadv_global;
				static siox_component_activity * preadv_network;
static siox_component_activity * pwritev64_global;
				static siox_component_activity * pwritev64_network;
static siox_component_activity * preadv64_global;
				static siox_component_activity * preadv64_network;
static siox_component_activity * cvsync;
static siox_component_activity * cvfsync;
static siox_component_activity * cvfdatasync;
static siox_component_activity * cvlseek;
static siox_component_activity * cvposix_fadvise;
static siox_component_activity * cvremove;
static siox_component_activity * cvrename;
int stat(const char *path, struct stat *buf){ return __xstat64(1, path, buf); }
static siox_component_activity * cv__xstat64;
int lstat(const char *path, struct stat *buf){ return __lxstat64(1, path, buf); }
static siox_component_activity * cv__lxstat64;
int fstat(int fd, struct stat *buf){ return __fxstat64(1, fd, buf); }
static siox_component_activity * cv__fxstat64;
static siox_component_activity * cv__fxstat;
static siox_component_activity * mmap_global;
				static siox_component_activity * mmap_network;
static siox_component_activity * mmap64_global;
				static siox_component_activity * mmap64_network;
static siox_component_activity * cvfopen;
static siox_component_activity * cvfopen64;
static siox_component_activity * cvfdopen;
static siox_component_activity * cvfileno;
static siox_component_activity * cvfreopen;
static siox_component_activity * cvtmpfile;
static siox_component_activity * cvfclose;
static siox_component_activity * cvfflush;
static siox_component_activity * cvfgetc;
static siox_component_activity * cvgetc;
static siox_component_activity * cvfputc;
static siox_component_activity * cvputc;
static siox_component_activity * cvfgets;
static siox_component_activity * cvfputs;
static siox_component_activity * cvfread;
static siox_component_activity * cvfwrite;
static siox_component_activity * cvfseeko;
static siox_component_activity * cvfseek;
static siox_component_activity * cvsetbuf;
static siox_component_activity * cvsetvbuf;
static siox_component_activity * cvunlink;
static siox_component_activity * cvvfprintf;
static siox_component_activity * cvvfscanf;
static siox_component_activity * cvfscanf;
static siox_component_activity * cvfprintf;
static siox_component_activity * cvaio_read;
static siox_component_activity * cvaio_write;
static siox_component_activity * cvlio_listio;
static siox_component_activity * cvaio_suspend;
static siox_component_activity * cvaio_cancel;
static siox_component_activity * cvlockf;
static siox_component_activity * cvflock;
static siox_component_activity * cvsocket;
static siox_component_activity * cvsetsockopt;
static siox_component_activity * cvpipe;
static siox_component_activity * cvpipe2;
static siox_component_activity * cvsocketpair;
static siox_component_activity * cvaccept;
static siox_component_activity * cvaccept4;


static void sioxFinal() __attribute__((destructor));
static void sioxInit() __attribute__((constructor));
#include <dlfcn.h>

static char* dllib = RTLD_NEXT;
static int (*__real_open) (const char *pathname, int flags, mode_t mode);
static int (*__real_creat) (const char *  pathname , mode_t  mode );
static int (*__real_open64) (const char *pathname, int flags, mode_t mode);
static int (*__real_creat64) (const char *  pathname , mode_t  mode );
static int (*__real_close) (int  fd );
static int (*__real_dup) (int  fd );
static int (*__real_dup2) (int  oldfd , int  newfd );
static int (*__real_dup3) (int  oldfd , int  newfd , int  flags );
static ssize_t (*__real_sendfile) (int  out_fd , int  in_fd , off_t * offset , size_t  count );
static ssize_t (*__real_write) (int  fd , const void *  buf , size_t  count );
static ssize_t (*__real_read) (int  fd , void *  buf , size_t  count );
static ssize_t (*__real_writev) (int  fd , const struct iovec *  iov , int  iovcnt );
static ssize_t (*__real_readv) (int  fd , const struct iovec *  iov , int  iovcnt );
static ssize_t (*__real_pwrite) (int  fd , const void *  buf , size_t  count , off_t  offset );
static ssize_t (*__real_pread) (int  fd , void *  buf , size_t  count , off_t  offset );
static ssize_t (*__real_pwrite64) (int  fd , const void *  buf , size_t  count , off_t  offset );
static ssize_t (*__real_pread64) (int  fd , void *  buf , size_t  count , off_t  offset );
static ssize_t (*__real_pwritev) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset );
static ssize_t (*__real_preadv) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset );
static ssize_t (*__real_pwritev64) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset );
static ssize_t (*__real_preadv64) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset );
static void (*__real_sync) (void  );
static int (*__real_fsync) (int  fd );
static int (*__real_fdatasync) (int  fd );
static off_t (*__real_lseek) (int  fd , off_t  offset , int  whence );
static int (*__real_posix_fadvise) (int  fd , off_t  offset , off_t  len , int  advise );
static int (*__real_remove) (const char *  filename );
static int (*__real_rename) (const char *  oldname , const char *  newname );
static int (*__real___xstat64) (int  __ver , const char *  path , struct stat64 *  buf );
static int (*__real___lxstat64) (int  __ver , const char *  path , struct stat64 *  buf );
static int (*__real___fxstat64) (int  __ver , int  fd , struct stat64 *  buf );
static int (*__real___fxstat) (int  __ver , int  fd , struct stat *  buf );
static void * (*__real_mmap) (void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset );
static void * (*__real_mmap64) (void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset );
static FILE * (*__real_fopen) (const char *  filename , const char *  mode );
static FILE * (*__real_fopen64) (const char *  filename , const char *  mode );
static FILE * (*__real_fdopen) (int  fd , const char *  mode );
static int (*__real_fileno) (FILE *  stream );
static FILE * (*__real_freopen) (const char *  filename , const char *  mode , FILE *  stream );
static FILE * (*__real_tmpfile) (void  );
static int (*__real_fclose) (FILE *  stream );
static int (*__real_fflush) (FILE *  stream );
static int (*__real_fgetc) (FILE *  stream );
static int (*__real_getc) (FILE *  stream );
static int (*__real_fputc) (int  character , FILE *  stream );
static int (*__real_putc) (int  character , FILE *  stream );
static char * (*__real_fgets) (char *  str , int  num , FILE *  stream );
static int (*__real_fputs) (const char *  str , FILE *  stream );
static size_t (*__real_fread) (void *  ptr , size_t  size , size_t  count , FILE *  stream );
static size_t (*__real_fwrite) (const void *  ptr , size_t  size , size_t  count , FILE *  stream );
static int (*__real_fseeko) (FILE * stream , off_t  offset , int  whence );
static int (*__real_fseek) (FILE * stream , long  offset , int  whence );
static void (*__real_setbuf) (FILE *  stream , char *  buffer );
static int (*__real_setvbuf) (FILE *  stream , char *  buffer , int  mode , size_t  size );
static int (*__real_unlink) (const char * pathname );
static int (*__real_vfprintf) (FILE *  stream , const char *  format , va_list  arg );
static int (*__real_vfscanf) (FILE *  stream , const char *  format , va_list  arg );
static int (*__real_fscanf) (FILE *  stream , const char *  format , ...  );
static int (*__real_fprintf) (FILE *  stream , const char *  format , ...  );
static int (*__real_aio_read) (struct aiocb *  cb );
static int (*__real_aio_write) (struct aiocb *  cb );
static int (*__real_lio_listio) (int  mode , struct aiocb * const  aiocb_list [], int  nitems , struct sigevent *  sevp );
static int (*__real_aio_suspend) (const struct aiocb * const  aiocb_list [], int  nitems , const struct timespec *  timeout );
static int (*__real_aio_cancel) (int  fd , struct aiocb *  aiocbp );
static pid_t (*__real_fork) (void  );
static int (*__real_lockf) (int  fd , int  cmd , off_t  len );
static int (*__real_flock) (int  fd , int  operation );
static int (*__real_socket) (int  domain , int  type , int  protocol );
static int (*__real_setsockopt) (int  sockfd , int  level , int  optname , const void * optval , socklen_t  optlen );
static int (*__real_pipe) (int  pipefd [2]);
static int (*__real_pipe2) (int  pipefd [2], int  flags );
static int (*__real_socketpair) (int  domain , int  type , int  protocol , int  sv [2]);
static int (*__real_accept) (int  sockfd , struct sockaddr * addr , socklen_t * addrlen );
static int (*__real_accept4) (int  sockfd , struct sockaddr * addr , socklen_t * addrlen , int  flags );
	static int initialized_dlsym = 0;


static void sioxSymbolInit() {
initialized_dlsym = 1;
__real_open = (int (*) (const char *  pathname , int  flags , ...  )) dlsym(dllib, (const char*) "open");
__real_creat = (int (*) (const char *  pathname , mode_t  mode )) dlsym(dllib, (const char*) "creat");
__real_open64 = (int (*) (const char *  pathname , int  flags , ...  )) dlsym(dllib, (const char*) "open");
__real_creat64 = (int (*) (const char *  pathname , mode_t  mode )) dlsym(dllib, (const char*) "creat64");
__real_close = (int (*) (int  fd )) dlsym(dllib, (const char*) "close");
__real_dup = (int (*) (int  fd )) dlsym(dllib, (const char*) "dup");
__real_dup2 = (int (*) (int  oldfd , int  newfd )) dlsym(dllib, (const char*) "dup2");
__real_dup3 = (int (*) (int  oldfd , int  newfd , int  flags )) dlsym(dllib, (const char*) "dup3");
__real_sendfile = (ssize_t (*) (int  out_fd , int  in_fd , off_t * offset , size_t  count )) dlsym(dllib, (const char*) "sendfile");
__real_write = (ssize_t (*) (int  fd , const void *  buf , size_t  count )) dlsym(dllib, (const char*) "write");
__real_read = (ssize_t (*) (int  fd , void *  buf , size_t  count )) dlsym(dllib, (const char*) "read");
__real_writev = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt )) dlsym(dllib, (const char*) "writev");
__real_readv = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt )) dlsym(dllib, (const char*) "readv");
__real_pwrite = (ssize_t (*) (int  fd , const void *  buf , size_t  count , off_t  offset )) dlsym(dllib, (const char*) "pwrite");
__real_pread = (ssize_t (*) (int  fd , void *  buf , size_t  count , off_t  offset )) dlsym(dllib, (const char*) "pread");
__real_pwrite64 = (ssize_t (*) (int  fd , const void *  buf , size_t  count , off_t  offset )) dlsym(dllib, (const char*) "pwrite64");
__real_pread64 = (ssize_t (*) (int  fd , void *  buf , size_t  count , off_t  offset )) dlsym(dllib, (const char*) "pread64");
__real_pwritev = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )) dlsym(dllib, (const char*) "pwritev");
__real_preadv = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )) dlsym(dllib, (const char*) "preadv");
__real_pwritev64 = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )) dlsym(dllib, (const char*) "pwritev64");
__real_preadv64 = (ssize_t (*) (int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )) dlsym(dllib, (const char*) "preadv64");
__real_sync = (void (*) (void  )) dlsym(dllib, (const char*) "sync");
__real_fsync = (int (*) (int  fd )) dlsym(dllib, (const char*) "fsync");
__real_fdatasync = (int (*) (int  fd )) dlsym(dllib, (const char*) "fdatasync");
__real_lseek = (off_t (*) (int  fd , off_t  offset , int  whence )) dlsym(dllib, (const char*) "lseek");
__real_posix_fadvise = (int (*) (int  fd , off_t  offset , off_t  len , int  advise )) dlsym(dllib, (const char*) "posix_fadvise");
__real_remove = (int (*) (const char *  filename )) dlsym(dllib, (const char*) "remove");
__real_rename = (int (*) (const char *  oldname , const char *  newname )) dlsym(dllib, (const char*) "rename");
__real___xstat64 = (int (*) (int  __ver , const char *  path , struct stat64 *  buf )) dlsym(dllib, (const char*) "__xstat64");
__real___lxstat64 = (int (*) (int  __ver , const char *  path , struct stat64 *  buf )) dlsym(dllib, (const char*) "__lxstat64");
__real___fxstat64 = (int (*) (int  __ver , int  fd , struct stat64 *  buf )) dlsym(dllib, (const char*) "__fxstat64");
__real___fxstat = (int (*) (int  __ver , int  fd , struct stat *  buf )) dlsym(dllib, (const char*) "__fxstat");
__real_mmap = (void * (*) (void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset )) dlsym(dllib, (const char*) "mmap");
__real_mmap64 = (void * (*) (void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset )) dlsym(dllib, (const char*) "mmap64");
__real_fopen = (FILE * (*) (const char *  filename , const char *  mode )) dlsym(dllib, (const char*) "fopen");
__real_fopen64 = (FILE * (*) (const char *  filename , const char *  mode )) dlsym(dllib, (const char*) "fopen64");
__real_fdopen = (FILE * (*) (int  fd , const char *  mode )) dlsym(dllib, (const char*) "fdopen");
__real_fileno = (int (*) (FILE *  stream )) dlsym(dllib, (const char*) "fileno");
__real_freopen = (FILE * (*) (const char *  filename , const char *  mode , FILE *  stream )) dlsym(dllib, (const char*) "freopen");
__real_tmpfile = (FILE * (*) (void  )) dlsym(dllib, (const char*) "tmpfile");
__real_fclose = (int (*) (FILE *  stream )) dlsym(dllib, (const char*) "fclose");
__real_fflush = (int (*) (FILE *  stream )) dlsym(dllib, (const char*) "fflush");
__real_fgetc = (int (*) (FILE *  stream )) dlsym(dllib, (const char*) "fgetc");
__real_getc = (int (*) (FILE *  stream )) dlsym(dllib, (const char*) "getc");
__real_fputc = (int (*) (int  character , FILE *  stream )) dlsym(dllib, (const char*) "fputc");
__real_putc = (int (*) (int  character , FILE *  stream )) dlsym(dllib, (const char*) "putc");
__real_fgets = (char * (*) (char *  str , int  num , FILE *  stream )) dlsym(dllib, (const char*) "fgets");
__real_fputs = (int (*) (const char *  str , FILE *  stream )) dlsym(dllib, (const char*) "fputs");
__real_fread = (size_t (*) (void *  ptr , size_t  size , size_t  count , FILE *  stream )) dlsym(dllib, (const char*) "fread");
__real_fwrite = (size_t (*) (const void *  ptr , size_t  size , size_t  count , FILE *  stream )) dlsym(dllib, (const char*) "fwrite");
__real_fseeko = (int (*) (FILE * stream , off_t  offset , int  whence )) dlsym(dllib, (const char*) "fseeko");
__real_fseek = (int (*) (FILE * stream , long  offset , int  whence )) dlsym(dllib, (const char*) "fseek");
__real_setbuf = (void (*) (FILE *  stream , char *  buffer )) dlsym(dllib, (const char*) "setbuf");
__real_setvbuf = (int (*) (FILE *  stream , char *  buffer , int  mode , size_t  size )) dlsym(dllib, (const char*) "setvbuf");
__real_unlink = (int (*) (const char * pathname )) dlsym(dllib, (const char*) "unlink");
__real_vfprintf = (int (*) (FILE *  stream , const char *  format , va_list  arg )) dlsym(dllib, (const char*) "vfprintf");
__real_vfscanf = (int (*) (FILE *  stream , const char *  format , va_list  arg )) dlsym(dllib, (const char*) "vfscanf");
__real_fscanf = (int (*) (FILE *  stream , const char *  format , ...  )) dlsym(dllib, (const char*) "vfscanf");
__real_fprintf = (int (*) (FILE *  stream , const char *  format , ...  )) dlsym(dllib, (const char*) "vfprintf");
__real_aio_read = (int (*) (struct aiocb *  cb )) dlsym(dllib, (const char*) "aio_read");
__real_aio_write = (int (*) (struct aiocb *  cb )) dlsym(dllib, (const char*) "aio_write");
__real_lio_listio = (int (*) (int  mode , struct aiocb * const  aiocb_list [], int  nitems , struct sigevent *  sevp )) dlsym(dllib, (const char*) "lio_listio");
__real_aio_suspend = (int (*) (const struct aiocb * const  aiocb_list [], int  nitems , const struct timespec *  timeout )) dlsym(dllib, (const char*) "aio_suspend");
__real_aio_cancel = (int (*) (int  fd , struct aiocb *  aiocbp )) dlsym(dllib, (const char*) "aio_cancel");
__real_fork = (pid_t (*) (void  )) dlsym(dllib, (const char*) "fork");
__real_lockf = (int (*) (int  fd , int  cmd , off_t  len )) dlsym(dllib, (const char*) "lockf");
__real_flock = (int (*) (int  fd , int  operation )) dlsym(dllib, (const char*) "flock");
__real_socket = (int (*) (int  domain , int  type , int  protocol )) dlsym(dllib, (const char*) "socket");
__real_setsockopt = (int (*) (int  sockfd , int  level , int  optname , const void * optval , socklen_t  optlen )) dlsym(dllib, (const char*) "setsockopt");
__real_pipe = (int (*) (int  pipefd [2])) dlsym(dllib, (const char*) "pipe");
__real_pipe2 = (int (*) (int  pipefd [2], int  flags )) dlsym(dllib, (const char*) "pipe2");
__real_socketpair = (int (*) (int  domain , int  type , int  protocol , int  sv [2])) dlsym(dllib, (const char*) "socketpair");
__real_accept = (int (*) (int  sockfd , struct sockaddr * addr , socklen_t * addrlen )) dlsym(dllib, (const char*) "accept");
__real_accept4 = (int (*) (int  sockfd , struct sockaddr * addr , socklen_t * addrlen , int  flags )) dlsym(dllib, (const char*) "accept4");
}

static void sioxInit() {

	if(initialized_dlsym == 0) sioxSymbolInit();
	if ( siox_is_monitoring_permanently_disabled() || global_component ){
				return; 
		}		
      
		global_uid = siox_system_information_lookup_interface_id("POSIX", "");

		// avoid double instrumentation with DLSYM and LD_PRELOAD
		if ( siox_component_is_registered( global_uid ) ){
			fprintf(stderr, "WARNING: layer '%s/%s' is already instrumented, do not use LD_PRELOAD again! Most likely the application breaks.\n", "POSIX", "");
			return;
		}
		global_component = siox_component_register(global_uid, """");
		siox_register_initialization_signal(sioxInit);
      siox_register_termination_signal(sioxFinal);
	if ( siox_is_monitoring_permanently_disabled() || network_component ){
				return; 
		}		
      
		network_uid = siox_system_information_lookup_interface_id("POSIX_Network", "");

		// avoid double instrumentation with DLSYM and LD_PRELOAD
		if ( siox_component_is_registered( network_uid ) ){
			fprintf(stderr, "WARNING: layer '%s/%s' is already instrumented, do not use LD_PRELOAD again! Most likely the application breaks.\n", "POSIX_Network", "");
			return;
		}
		network_component = siox_component_register(network_uid, """");
		siox_register_initialization_signal(sioxInit);
      siox_register_termination_signal(sioxFinal);
	bytesToRead = siox_ontology_register_attribute( "POSIX", "quantity/BytesToRead", SIOX_STORAGE_64_BIT_UINTEGER ); assert(bytesToRead != NULL);
	bytesToWrite = siox_ontology_register_attribute( "POSIX", "quantity/BytesToWrite", SIOX_STORAGE_64_BIT_UINTEGER ); assert(bytesToWrite != NULL);
	filePosition = siox_ontology_register_attribute( "POSIX", "file/position", SIOX_STORAGE_64_BIT_UINTEGER ); assert(filePosition != NULL);
	fileExtent = siox_ontology_register_attribute( "POSIX", "file/extent", SIOX_STORAGE_64_BIT_UINTEGER ); assert(fileExtent != NULL);
	fileMemoryRegions = siox_ontology_register_attribute( "POSIX", "quantity/memoryRegions", SIOX_STORAGE_32_BIT_INTEGER ); assert(fileMemoryRegions != NULL);
	fileOpenFlags = siox_ontology_register_attribute( "POSIX", "hints/openFlags", SIOX_STORAGE_32_BIT_UINTEGER ); assert(fileOpenFlags != NULL);
	fileName = siox_ontology_register_attribute( "POSIX", "descriptor/filename", SIOX_STORAGE_STRING ); assert(fileName != NULL);
	fileSystem = siox_ontology_register_attribute( "Global", "descriptor/filesystem", SIOX_STORAGE_32_BIT_UINTEGER ); assert(fileSystem != NULL);
	fileHandle = siox_ontology_register_attribute( "POSIX", "descriptor/filehandle", SIOX_STORAGE_32_BIT_UINTEGER ); assert(fileHandle != NULL);
	bytesWritten = siox_ontology_register_attribute( "POSIX", "quantity/BytesWritten", SIOX_STORAGE_64_BIT_UINTEGER ); assert(bytesWritten != NULL);
	bytesRead = siox_ontology_register_attribute( "POSIX", "quantity/BytesRead", SIOX_STORAGE_64_BIT_UINTEGER ); assert(bytesRead != NULL);
	fileAdviseExtent = siox_ontology_register_attribute( "POSIX", "hint/advise-extent", SIOX_STORAGE_64_BIT_UINTEGER ); assert(fileAdviseExtent != NULL);
	fileAdvise = siox_ontology_register_attribute( "POSIX", "hints/advise", SIOX_STORAGE_32_BIT_INTEGER ); assert(fileAdvise != NULL);
	fileBufferSize = siox_ontology_register_attribute( "POSIX", "hints/bufferSize", SIOX_STORAGE_64_BIT_INTEGER ); assert(fileBufferSize != NULL);
	fileBufferMode = siox_ontology_register_attribute( "POSIX", "hints/bufferMode", SIOX_STORAGE_32_BIT_INTEGER ); assert(fileBufferMode != NULL);
	activityHashTable_int = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free);
	activityHashTable_network_int = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free);
	activityHashTable_size = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free);
	cvopen = siox_component_register_activity( global_uid, "open" );
	cvcreat = siox_component_register_activity( global_uid, "creat" );
	cvopen64 = siox_component_register_activity( global_uid, "open" );
	cvcreat64 = siox_component_register_activity( global_uid, "creat" );
	close_global = siox_component_register_activity( global_uid, "close" );
	close_network = siox_component_register_activity( network_uid, "close" );
	dup_global = siox_component_register_activity( global_uid, "dup" );
	dup_network = siox_component_register_activity( network_uid, "dup" );
	dup2_global = siox_component_register_activity( global_uid, "dup2" );
	dup2_network = siox_component_register_activity( network_uid, "dup2" );
	dup3_global = siox_component_register_activity( global_uid, "dup3" );
	dup3_network = siox_component_register_activity( network_uid, "dup3" );
	cvsendfile = siox_component_register_activity( global_uid, "sendfile" );
	write_global = siox_component_register_activity( global_uid, "write" );
	write_network = siox_component_register_activity( network_uid, "write" );
	read_global = siox_component_register_activity( global_uid, "read" );
	read_network = siox_component_register_activity( network_uid, "read" );
	writev_global = siox_component_register_activity( global_uid, "writev" );
	writev_network = siox_component_register_activity( network_uid, "writev" );
	readv_global = siox_component_register_activity( global_uid, "readv" );
	readv_network = siox_component_register_activity( network_uid, "readv" );
	pwrite_global = siox_component_register_activity( global_uid, "pwrite" );
	pwrite_network = siox_component_register_activity( network_uid, "pwrite" );
	pread_global = siox_component_register_activity( global_uid, "pread" );
	pread_network = siox_component_register_activity( network_uid, "pread" );
	pwrite64_global = siox_component_register_activity( global_uid, "pwrite64" );
	pwrite64_network = siox_component_register_activity( network_uid, "pwrite64" );
	pread64_global = siox_component_register_activity( global_uid, "pread64" );
	pread64_network = siox_component_register_activity( network_uid, "pread64" );
	pwritev_global = siox_component_register_activity( global_uid, "pwritev" );
	pwritev_network = siox_component_register_activity( network_uid, "pwritev" );
	preadv_global = siox_component_register_activity( global_uid, "preadv" );
	preadv_network = siox_component_register_activity( network_uid, "preadv" );
	pwritev64_global = siox_component_register_activity( global_uid, "pwritev64" );
	pwritev64_network = siox_component_register_activity( network_uid, "pwritev64" );
	preadv64_global = siox_component_register_activity( global_uid, "preadv64" );
	preadv64_network = siox_component_register_activity( network_uid, "preadv64" );
	cvsync = siox_component_register_activity( global_uid, "sync" );
	cvfsync = siox_component_register_activity( global_uid, "fsync" );
	cvfdatasync = siox_component_register_activity( global_uid, "fdatasync" );
	cvlseek = siox_component_register_activity( global_uid, "lseek" );
	cvposix_fadvise = siox_component_register_activity( global_uid, "posix_fadvise" );
	cvremove = siox_component_register_activity( global_uid, "remove" );
	cvrename = siox_component_register_activity( global_uid, "rename" );
	cv__xstat64 = siox_component_register_activity( global_uid, "stat" );
	cv__lxstat64 = siox_component_register_activity( global_uid, "lstat" );
	cv__fxstat64 = siox_component_register_activity( global_uid, "fstat" );
	cv__fxstat = siox_component_register_activity( global_uid, "fstat" );
	mmap_global = siox_component_register_activity( global_uid, "mmap" );
	mmap_network = siox_component_register_activity( network_uid, "mmap" );
	mmap64_global = siox_component_register_activity( global_uid, "mmap64" );
	mmap64_network = siox_component_register_activity( network_uid, "mmap64" );
	cvfopen = siox_component_register_activity( global_uid, "fopen" );
	cvfopen64 = siox_component_register_activity( global_uid, "fopen64" );
	cvfdopen = siox_component_register_activity( global_uid, "fdopen" );
	cvfileno = siox_component_register_activity( global_uid, "fileno" );
	cvfreopen = siox_component_register_activity( global_uid, "freopen" );
	cvtmpfile = siox_component_register_activity( global_uid, "tmpfile" );
	cvfclose = siox_component_register_activity( global_uid, "fclose" );
	cvfflush = siox_component_register_activity( global_uid, "fflush" );
	cvfgetc = siox_component_register_activity( global_uid, "fgetc" );
	cvgetc = siox_component_register_activity( global_uid, "getc" );
	cvfputc = siox_component_register_activity( global_uid, "fputc" );
	cvputc = siox_component_register_activity( global_uid, "putc" );
	cvfgets = siox_component_register_activity( global_uid, "fgets" );
	cvfputs = siox_component_register_activity( global_uid, "fputs" );
	cvfread = siox_component_register_activity( global_uid, "fread" );
	cvfwrite = siox_component_register_activity( global_uid, "fwrite" );
	cvfseeko = siox_component_register_activity( global_uid, "fseeko" );
	cvfseek = siox_component_register_activity( global_uid, "fseek" );
	cvsetbuf = siox_component_register_activity( global_uid, "setbuf" );
	cvsetvbuf = siox_component_register_activity( global_uid, "setvbuf" );
	cvunlink = siox_component_register_activity( global_uid, "unlink" );
	cvvfprintf = siox_component_register_activity( global_uid, "vfprintf" );
	cvvfscanf = siox_component_register_activity( global_uid, "vfscanf" );
	cvfscanf = siox_component_register_activity( global_uid, "fscanf" );
	cvfprintf = siox_component_register_activity( global_uid, "fprintf" );
	cvaio_read = siox_component_register_activity( global_uid, "aio_read" );
	cvaio_write = siox_component_register_activity( global_uid, "aio_write" );
	cvlio_listio = siox_component_register_activity( global_uid, "lio_listio" );
	cvaio_suspend = siox_component_register_activity( global_uid, "aio_suspend" );
	cvaio_cancel = siox_component_register_activity( global_uid, "aio_cancel" );
	cvlockf = siox_component_register_activity( global_uid, "lockf" );
	cvflock = siox_component_register_activity( global_uid, "flock" );
	cvsocket = siox_component_register_activity( network_uid, "socket" );
	cvsetsockopt = siox_component_register_activity( network_uid, "setsockopt" );
	cvpipe = siox_component_register_activity( network_uid, "pipe" );
	cvpipe2 = siox_component_register_activity( network_uid, "pipe2" );
	cvsocketpair = siox_component_register_activity( network_uid, "socketpair" );
	cvaccept = siox_component_register_activity( network_uid, "accept" );
	cvaccept4 = siox_component_register_activity( network_uid, "accept4" );
	global_layer_initialized = TRUE;
	network_layer_initialized = TRUE;
}

static void sioxFinal() {
	if (global_layer_initialized) { siox_component_unregister(global_component); global_component = NULL; global_layer_initialized = FALSE; }
	if (network_layer_initialized) { siox_component_unregister(network_component); network_component = NULL; network_layer_initialized = FALSE; }
}
int open(const char *  pathname , int  flags , ...  )
{
	va_list valist;
	va_start(valist, flags);
	int ret;
	mode_t mode = va_arg(valist,mode_t);
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvopen);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvopen );
	uint32_t translatedFlags = translatePOSIXFlagsToSIOX(flags);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	SET_FILENAME(pathname)
	siox_activity_start(sioxActivity);
	ret = (__real_open)(pathname,flags,mode);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	g_rw_lock_writer_lock(& lock_activityHashTable_int); 
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_open)(pathname,flags,mode);

 	}
	va_end(valist);
	return ret;
}

int creat(const char *  pathname , mode_t  mode )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvcreat);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvcreat );
	SET_FILENAME(pathname)
	siox_activity_start(sioxActivity);
	ret = (__real_creat)(pathname, mode);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_int); 
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_creat)(pathname, mode);

 	}
	return ret;
}

int open64(const char *  pathname , int  flags , ...  )
{
	va_list valist;
	va_start(valist, flags);
	int ret;
	mode_t mode = va_arg(valist,mode_t);
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvopen64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvopen64 );
	SET_FILENAME(pathname)
	uint32_t translatedFlags = translatePOSIXFlagsToSIOX(flags);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	siox_activity_start(sioxActivity);
	ret = (__real_open64)(pathname,flags,mode);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_int); 
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_open64)(pathname,flags,mode);

 	}
	va_end(valist);
	return ret;
}

int creat64(const char *  pathname , mode_t  mode )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvcreat64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvcreat64 );
	SET_FILENAME(pathname)
	siox_activity_start(sioxActivity);
	ret = (__real_creat64)(pathname, mode);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_int); 
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_creat64)(pathname, mode);

 	}
	return ret;
}

int close(int  fd )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, close_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, close_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, close_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_start(sioxActivity);
	ret = (__real_close)(fd);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_int);
    	g_hash_table_remove( activityHashTable_int, GINT_TO_POINTER(fd) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int);
    	g_hash_table_remove( activityHashTable_network_int, GINT_TO_POINTER(fd) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_close)(fd);

 	}
	return ret;
}

int dup(int  fd )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, dup_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, dup_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, dup_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_start(sioxActivity);
	ret = (__real_dup)(fd);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	if ( parent != NULL ){
    	siox_activity_ID * nID = malloc(sizeof(siox_activity_ID));
    	memcpy(nID, parent, sizeof(siox_activity_ID));
    	g_rw_lock_writer_lock(& lock_activityHashTable_int);
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), nID );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	}
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_dup)(fd);

 	}
	return ret;
}

int dup2(int  oldfd , int  newfd )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(oldfd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(oldfd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, dup2_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, dup2_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, dup2_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_start(sioxActivity);
	ret = (__real_dup2)(oldfd, newfd);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	if ( parent != NULL ){
    	siox_activity_ID * nID = malloc(sizeof(siox_activity_ID));
    	memcpy(nID, parent, sizeof(siox_activity_ID));
    	g_rw_lock_writer_lock(& lock_activityHashTable_int);
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(newfd), nID );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	}
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_dup2)(oldfd, newfd);

 	}
	return ret;
}

int dup3(int  oldfd , int  newfd , int  flags )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(oldfd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(oldfd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, dup3_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, dup3_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, dup3_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_start(sioxActivity);
	ret = (__real_dup3)(oldfd, newfd, flags);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	if ( parent != NULL ){
    	siox_activity_ID * nID = malloc(sizeof(siox_activity_ID));
    	memcpy(nID, parent, sizeof(siox_activity_ID));
    	g_rw_lock_writer_lock(& lock_activityHashTable_int);
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(newfd), nID );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	}
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_dup3)(oldfd, newfd, flags);

 	}
	return ret;
}

ssize_t sendfile(int  out_fd , int  in_fd , off_t * offset , size_t  count )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvsendfile);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvsendfile );
	siox_activity_set_attribute( sioxActivity, bytesToWrite, &count );
	siox_activity_set_attribute( sioxActivity, fileHandle, &out_fd );
	siox_activity_set_attribute( sioxActivity, fileHandle, &in_fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(out_fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(in_fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_sendfile)(out_fd, in_fd, offset, count);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_sendfile)(out_fd, in_fd, offset, count);

 	}
	return ret;
}

ssize_t write(int  fd , const void *  buf , size_t  count )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, write_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, write_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, write_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, bytesToWrite, &count );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_start(sioxActivity);
	ret = (__real_write)(fd, buf, count);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_write)(fd, buf, count);

 	}
	return ret;
}

ssize_t read(int  fd , void *  buf , size_t  count )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, read_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, read_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, read_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, bytesToRead, &count );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_start(sioxActivity);
	ret = (__real_read)(fd, buf, count);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_read)(fd, buf, count);

 	}
	return ret;
}

ssize_t writev(int  fd , const struct iovec *  iov , int  iovcnt )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, writev_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, writev_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, writev_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_start(sioxActivity);
	ret = (__real_writev)(fd, iov, iovcnt);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_writev)(fd, iov, iovcnt);

 	}
	return ret;
}

ssize_t readv(int  fd , const struct iovec *  iov , int  iovcnt )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, readv_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, readv_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, readv_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_start(sioxActivity);
	ret = (__real_readv)(fd, iov, iovcnt);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_readv)(fd, iov, iovcnt);

 	}
	return ret;
}

ssize_t pwrite(int  fd , const void *  buf , size_t  count , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pwrite_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pwrite_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pwrite_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, bytesToWrite, &count );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pwrite)(fd, buf, count, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pwrite)(fd, buf, count, offset);

 	}
	return ret;
}

ssize_t pread(int  fd , void *  buf , size_t  count , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pread_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pread_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pread_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, bytesToRead, &count );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pread)(fd, buf, count, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pread)(fd, buf, count, offset);

 	}
	return ret;
}

ssize_t pwrite64(int  fd , const void *  buf , size_t  count , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pwrite64_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pwrite64_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pwrite64_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, bytesToWrite, &count );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pwrite64)(fd, buf, count, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pwrite64)(fd, buf, count, offset);

 	}
	return ret;
}

ssize_t pread64(int  fd , void *  buf , size_t  count , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pread64_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pread64_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pread64_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, bytesToRead, &count );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pread64)(fd, buf, count, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pread64)(fd, buf, count, offset);

 	}
	return ret;
}

ssize_t pwritev(int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pwritev_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pwritev_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pwritev_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pwritev)(fd, iov, iovcnt, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pwritev)(fd, iov, iovcnt, offset);

 	}
	return ret;
}

ssize_t preadv(int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, preadv_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, preadv_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, preadv_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_preadv)(fd, iov, iovcnt, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_preadv)(fd, iov, iovcnt, offset);

 	}
	return ret;
}

ssize_t pwritev64(int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, pwritev64_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, pwritev64_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, pwritev64_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_pwritev64)(fd, iov, iovcnt, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pwritev64)(fd, iov, iovcnt, offset);

 	}
	return ret;
}

ssize_t preadv64(int  fd , const struct iovec *  iov , int  iovcnt , off_t  offset )
{
	ssize_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, preadv64_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, preadv64_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, preadv64_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileMemoryRegions, &iovcnt );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_preadv64)(fd, iov, iovcnt, offset);

	int errsv = errno;
		    if ( ret==(size_t)-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesRead, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_preadv64)(fd, iov, iovcnt, offset);

 	}
	return ret;
}

void sync(void  )
{
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvsync);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvsync );
	siox_activity_start(sioxActivity);
	(__real_sync)();

	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	(__real_sync)();

 	}

}

int fsync(int  fd )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfsync);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfsync );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_fsync)(fd);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fsync)(fd);

 	}
	return ret;
}

int fdatasync(int  fd )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfdatasync);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfdatasync );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_fdatasync)(fd);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fdatasync)(fd);

 	}
	return ret;
}

off_t lseek(int  fd , off_t  offset , int  whence )
{
	off_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvlseek);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvlseek );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_set_attribute( sioxActivity, filePosition, &ret );
	siox_activity_start(sioxActivity);
	ret = (__real_lseek)(fd, offset, whence);

	int errsv = errno;
		    if ( ret == (off_t) -1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_lseek)(fd, offset, whence);

 	}
	return ret;
}

int posix_fadvise(int  fd , off_t  offset , off_t  len , int  advise )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvposix_fadvise);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvposix_fadvise );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_set_attribute( sioxActivity, fileAdviseExtent, &len );
	siox_activity_set_attribute( sioxActivity, fileAdvise, &advise );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_posix_fadvise)(fd, offset, len, advise);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_posix_fadvise)(fd, offset, len, advise);

 	}
	return ret;
}

int remove(const char *  filename )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvremove);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvremove );
	SET_FILENAME(filename)
	siox_activity_start(sioxActivity);
	ret = (__real_remove)(filename);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_remove)(filename);

 	}
	return ret;
}

int rename(const char *  oldname , const char *  newname )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvrename);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvrename );
	SET_FILENAME(oldname)
	siox_activity_start(sioxActivity);
	ret = (__real_rename)(oldname, newname);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_rename)(oldname, newname);

 	}
	return ret;
}

int __xstat64(int  __ver , const char *  path , struct stat64 *  buf )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cv__xstat64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cv__xstat64 );
	SET_FILENAME(path)
	siox_activity_start(sioxActivity);
	ret = (__real___xstat64)(__ver, path, buf);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real___xstat64)(__ver, path, buf);

 	}
	return ret;
}

int __lxstat64(int  __ver , const char *  path , struct stat64 *  buf )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cv__lxstat64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cv__lxstat64 );
	SET_FILENAME(path)
	siox_activity_start(sioxActivity);
	ret = (__real___lxstat64)(__ver, path, buf);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real___lxstat64)(__ver, path, buf);

 	}
	return ret;
}

int __fxstat64(int  __ver , int  fd , struct stat64 *  buf )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cv__fxstat64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cv__fxstat64 );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real___fxstat64)(__ver, fd, buf);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real___fxstat64)(__ver, fd, buf);

 	}
	return ret;
}

int __fxstat(int  __ver , int  fd , struct stat *  buf )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cv__fxstat);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cv__fxstat );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real___fxstat)(__ver, fd, buf);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real___fxstat)(__ver, fd, buf);

 	}
	return ret;
}

void * mmap(void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset )
{
	void * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, mmap_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, mmap_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, mmap_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, fileExtent, &length );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_mmap)(address, length, protect, flags, fd, offset);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_mmap)(address, length, protect, flags, fd, offset);

 	}
	return ret;
}

void * mmap64(void *  address , size_t  length , int  protect , int  flags , int  fd , off_t  offset )
{
	void * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	g_rw_lock_reader_lock(& lock_activityHashTable_int); 
			siox_activity_ID * parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) ); 
			g_rw_lock_reader_unlock(& lock_activityHashTable_int);
			// now decide to which component the activity parent belongs to
			// we expect it is likely to belong to the first component
			siox_activity * sioxActivity = NULL;
			
			if ( parent == NULL ){
				// check if it belongs to the other component
				g_rw_lock_reader_lock(& lock_activityHashTable_network_int); 
			   parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_network_int, GINT_TO_POINTER(fd) ); 
				g_rw_lock_reader_unlock(& lock_activityHashTable_network_int);

				if( parent != NULL ){
					sioxActivity = siox_activity_begin( network_component, mmap64_network );					
				}else{
				   // unknown so we keep the first component
					sioxActivity = siox_activity_begin( global_component, mmap64_global );
				}
			}else{
				sioxActivity = siox_activity_begin( global_component, mmap64_global );
			}

	    	
	    	siox_activity_link_to_parent( sioxActivity, parent );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_set_attribute( sioxActivity, fileExtent, &length );
	siox_activity_set_attribute( sioxActivity, filePosition, &offset );
	siox_activity_start(sioxActivity);
	ret = (__real_mmap64)(address, length, protect, flags, fd, offset);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_mmap64)(address, length, protect, flags, fd, offset);

 	}
	return ret;
}

FILE * fopen(const char *  filename , const char *  mode )
{
	FILE * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfopen);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfopen );
	SET_FILENAME(filename)
	uint32_t translatedFlags = translateFILEFlagsToSIOX(mode);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	siox_activity_start(sioxActivity);
	ret = (__real_fopen)(filename, mode);

	int errsv = errno;
		    if ( ret==NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	int fd = (ret != 0) ? fileno(ret) : 0;
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	g_rw_lock_writer_lock(& lock_activityHashTable_size); 
    	g_hash_table_insert( activityHashTable_size, GSIZE_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fopen)(filename, mode);

 	}
	return ret;
}

FILE * fopen64(const char *  filename , const char *  mode )
{
	FILE * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfopen64);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfopen64 );
	SET_FILENAME(filename)
	uint32_t translatedFlags = translateFILEFlagsToSIOX(mode);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	siox_activity_start(sioxActivity);
	ret = (__real_fopen64)(filename, mode);

	int errsv = errno;
		    if ( ret==NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_size); 
    	g_hash_table_insert( activityHashTable_size, GSIZE_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	int fd = (ret != 0) ? fileno(ret) : 0;
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fopen64)(filename, mode);

 	}
	return ret;
}

FILE * fdopen(int  fd , const char *  mode )
{
	FILE * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfdopen);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfdopen );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	uint32_t translatedFlags = translateFILEFlagsToSIOX(mode);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	siox_activity_start(sioxActivity);
	ret = (__real_fdopen)(fd, mode);

	int errsv = errno;
		    if ( ret==NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_size); 
    	g_hash_table_insert( activityHashTable_size, GSIZE_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fdopen)(fd, mode);

 	}
	return ret;
}

int fileno(FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfileno);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfileno );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fileno)(stream);

	int errsv = errno;
		    if ( ret==-1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_int); 
    	g_hash_table_insert( activityHashTable_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fileno)(stream);

 	}
	return ret;
}

FILE * freopen(const char *  filename , const char *  mode , FILE *  stream )
{
	FILE * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfreopen);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfreopen );
	SET_FILENAME(filename)
	uint32_t translatedFlags = translateFILEFlagsToSIOX(mode);
	siox_activity_set_attribute( sioxActivity, fileOpenFlags, &translatedFlags );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_freopen)(filename, mode, stream);

	int errsv = errno;
		    if ( ret==NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_size);
    	g_hash_table_remove( activityHashTable_size, GSIZE_TO_POINTER(stream) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	g_rw_lock_writer_lock(& lock_activityHashTable_size); 
    	g_hash_table_insert( activityHashTable_size, GSIZE_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	int fd = (ret != 0) ? fileno(ret) : 0;
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_freopen)(filename, mode, stream);

 	}
	return ret;
}

FILE * tmpfile(void  )
{
	FILE * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvtmpfile);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvtmpfile );
	siox_activity_start(sioxActivity);
	ret = (__real_tmpfile)();

	int errsv = errno;
		    if ( ret==NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_size); 
    	g_hash_table_insert( activityHashTable_size, GSIZE_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	int fd = (ret != 0) ? fileno(ret) : 0;
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_tmpfile)();

 	}
	return ret;
}

int fclose(FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfclose);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfclose );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	int fd = fileno(stream);
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	siox_activity_start(sioxActivity);
	ret = (__real_fclose)(stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_size);
    	g_hash_table_remove( activityHashTable_size, GSIZE_TO_POINTER(stream) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_size);
	g_rw_lock_writer_lock(& lock_activityHashTable_int);
    	g_hash_table_remove( activityHashTable_int, GINT_TO_POINTER(fd) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_int);
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int);
    	g_hash_table_remove( activityHashTable_network_int, GINT_TO_POINTER(fd) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fclose)(stream);

 	}
	return ret;
}

int fflush(FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfflush);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfflush );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fflush)(stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fflush)(stream);

 	}
	return ret;
}

int fgetc(FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfgetc);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfgetc );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fgetc)(stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fgetc)(stream);

 	}
	return ret;
}

int getc(FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvgetc);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvgetc );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_getc)(stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_getc)(stream);

 	}
	return ret;
}

int fputc(int  character , FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfputc);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfputc );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fputc)(character, stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fputc)(character, stream);

 	}
	return ret;
}

int putc(int  character , FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvputc);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvputc );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_putc)(character, stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_putc)(character, stream);

 	}
	return ret;
}

char * fgets(char *  str , int  num , FILE *  stream )
{
	char * ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfgets);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfgets );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fgets)(str, num, stream);

	int errsv = errno;
		    if ( ret == NULL ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fgets)(str, num, stream);

 	}
	return ret;
}

int fputs(const char *  str , FILE *  stream )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfputs);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfputs );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fputs)(str, stream);

	int errsv = errno;
		    if ( ret == EOF ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fputs)(str, stream);

 	}
	return ret;
}

size_t fread(void *  ptr , size_t  size , size_t  count , FILE *  stream )
{
	size_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfread);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfread );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	uint64_t payload = count*size;
	siox_activity_set_attribute( sioxActivity, bytesToRead, &payload );
	siox_activity_start(sioxActivity);
	ret = (__real_fread)(ptr, size, count, stream);

	int errsv = errno;
		    if ( ret != count ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	uint64_t posDelta = ret*size;
	siox_activity_set_attribute( sioxActivity, bytesRead, &posDelta );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fread)(ptr, size, count, stream);

 	}
	return ret;
}

size_t fwrite(const void *  ptr , size_t  size , size_t  count , FILE *  stream )
{
	size_t ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfwrite);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfwrite );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	uint64_t payload = count*size;
	siox_activity_set_attribute( sioxActivity, bytesToWrite, &payload );
	siox_activity_start(sioxActivity);
	ret = (__real_fwrite)(ptr, size, count, stream);

	int errsv = errno;
		    if ( ret != count ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	uint64_t posDelta = ret*size;
	siox_activity_set_attribute( sioxActivity, bytesWritten, &posDelta );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fwrite)(ptr, size, count, stream);

 	}
	return ret;
}

int fseeko(FILE * stream , off_t  offset , int  whence )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfseeko);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfseeko );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fseeko)(stream, offset, whence);

	int errsv = errno;
		    if ( ret != 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	uint64_t pos = (uint64_t) ftello(stream);
	siox_activity_set_attribute( sioxActivity, filePosition, &pos );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fseeko)(stream, offset, whence);

 	}
	return ret;
}

int fseek(FILE * stream , long  offset , int  whence )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfseek);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfseek );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fseek)(stream, offset, whence);

	int errsv = errno;
		    if ( ret != 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	uint64_t pos = (uint64_t) ftell(stream);
	siox_activity_set_attribute( sioxActivity, filePosition, &pos );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fseek)(stream, offset, whence);

 	}
	return ret;
}

void setbuf(FILE *  stream , char *  buffer )
{
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvsetbuf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvsetbuf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	(__real_setbuf)(stream, buffer);

	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	(__real_setbuf)(stream, buffer);

 	}

}

int setvbuf(FILE *  stream , char *  buffer , int  mode , size_t  size )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvsetvbuf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvsetvbuf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_set_attribute( sioxActivity, fileBufferSize, &size );
	siox_activity_set_attribute( sioxActivity, fileBufferMode, &mode );
	siox_activity_start(sioxActivity);
	ret = (__real_setvbuf)(stream, buffer, mode, size);

	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_setvbuf)(stream, buffer, mode, size);

 	}
	return ret;
}

int unlink(const char * pathname )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvunlink);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvunlink );
	SET_FILENAME(pathname) ;
	siox_activity_start(sioxActivity);
	ret = (__real_unlink)(pathname);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_unlink)(pathname);

 	}
	return ret;
}

int vfprintf(FILE *  stream , const char *  format , va_list  arg )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvvfprintf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvvfprintf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_vfprintf)(stream, format, arg);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, bytesWritten, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_vfprintf)(stream, format, arg);

 	}
	return ret;
}

int vfscanf(FILE *  stream , const char *  format , va_list  arg )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvvfscanf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvvfscanf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_vfscanf)(stream, format, arg);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_vfscanf)(stream, format, arg);

 	}
	return ret;
}

int fscanf(FILE *  stream , const char *  format , ...  )
{
	va_list valist;
	va_start(valist, format);
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfscanf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfscanf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fscanf)(stream,format,valist);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fscanf)(stream,format,valist);

 	}
	va_end(valist);
	return ret;
}

int fprintf(FILE *  stream , const char *  format , ...  )
{
	va_list valist;
	va_start(valist, format);
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvfprintf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvfprintf );
	g_rw_lock_reader_lock(& lock_activityHashTable_size);
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_size, GSIZE_TO_POINTER(stream) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_size);
        siox_activity_link_to_parent( sioxActivity, Parent );
	siox_activity_start(sioxActivity);
	ret = (__real_fprintf)(stream,format,valist);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_fprintf)(stream,format,valist);

 	}
	va_end(valist);
	return ret;
}

int aio_read(struct aiocb *  cb )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvaio_read);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvaio_read );
	siox_activity_start(sioxActivity);
	ret = (__real_aio_read)(cb);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_aio_read)(cb);

 	}
	return ret;
}

int aio_write(struct aiocb *  cb )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvaio_write);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvaio_write );
	siox_activity_start(sioxActivity);
	ret = (__real_aio_write)(cb);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_aio_write)(cb);

 	}
	return ret;
}

int lio_listio(int  mode , struct aiocb * const  aiocb_list [], int  nitems , struct sigevent *  sevp )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvlio_listio);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvlio_listio );
	siox_activity_start(sioxActivity);
	ret = (__real_lio_listio)(mode, aiocb_list, nitems, sevp);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_lio_listio)(mode, aiocb_list, nitems, sevp);

 	}
	return ret;
}

int aio_suspend(const struct aiocb * const  aiocb_list [], int  nitems , const struct timespec *  timeout )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvaio_suspend);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvaio_suspend );
	siox_activity_start(sioxActivity);
	ret = (__real_aio_suspend)(aiocb_list, nitems, timeout);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_aio_suspend)(aiocb_list, nitems, timeout);

 	}
	return ret;
}

int aio_cancel(int  fd , struct aiocb *  aiocbp )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvaio_cancel);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvaio_cancel );
	siox_activity_start(sioxActivity);
	ret = (__real_aio_cancel)(fd, aiocbp);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_aio_cancel)(fd, aiocbp);

 	}
	return ret;
}

pid_t fork(void  )
{
	pid_t ret;
	printf("Warning fork() called!\n");
	siox_handle_prepare_fork() ;
	ret = (__real_fork)();

	siox_handle_fork_complete(ret != 0);
	return ret;
}

int lockf(int  fd , int  cmd , off_t  len )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvlockf);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvlockf );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_lockf)(fd, cmd, len);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_lockf)(fd, cmd, len);

 	}
	return ret;
}

int flock(int  fd , int  operation )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && global_layer_initialized && siox_is_monitoring_enabled() ){
	assert(global_component);
	    	assert(cvflock);
	    	siox_activity * sioxActivity = siox_activity_begin( global_component, cvflock );
	siox_activity_set_attribute( sioxActivity, fileHandle, &fd );
	{
		g_rw_lock_reader_lock(& lock_activityHashTable_int); 
		siox_activity_ID * Parent = (siox_activity_ID*) g_hash_table_lookup( activityHashTable_int, GINT_TO_POINTER(fd) );
		g_rw_lock_reader_unlock(& lock_activityHashTable_int);
        siox_activity_link_to_parent( sioxActivity, Parent ); 
      }
	siox_activity_start(sioxActivity);
	ret = (__real_flock)(fd, operation);

	int errsv = errno;
		    if ( ret<0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_flock)(fd, operation);

 	}
	return ret;
}

int socket(int  domain , int  type , int  protocol )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvsocket);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvsocket );
	siox_activity_start(sioxActivity);
	ret = (__real_socket)(domain, type, protocol);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_socket)(domain, type, protocol);

 	}
	return ret;
}

int setsockopt(int  sockfd , int  level , int  optname , const void * optval , socklen_t  optlen )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvsetsockopt);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvsetsockopt );
	siox_activity_start(sioxActivity);
	ret = (__real_setsockopt)(sockfd, level, optname, optval, optlen);

	int errsv = errno;
		    if ( ret == -1 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	siox_activity_set_attribute( sioxActivity, fileHandle, &sockfd );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_setsockopt)(sockfd, level, optname, optval, optlen);

 	}
	return ret;
}

int pipe(int  pipefd [2])
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvpipe);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvpipe );
	siox_activity_start(sioxActivity);
	ret = (__real_pipe)(pipefd);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(pipefd[0]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(pipefd[1]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &pipefd[0] );
	siox_activity_set_attribute( sioxActivity, fileHandle, &pipefd[1] );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pipe)(pipefd);

 	}
	return ret;
}

int pipe2(int  pipefd [2], int  flags )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvpipe2);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvpipe2 );
	siox_activity_start(sioxActivity);
	ret = (__real_pipe2)(pipefd, flags);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(pipefd[0]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(pipefd[1]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &pipefd[0] );
	siox_activity_set_attribute( sioxActivity, fileHandle, &pipefd[1] );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_pipe2)(pipefd, flags);

 	}
	return ret;
}

int socketpair(int  domain , int  type , int  protocol , int  sv [2])
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvsocketpair);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvsocketpair );
	siox_activity_start(sioxActivity);
	ret = (__real_socketpair)(domain, type, protocol, sv);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(sv[0]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(sv[1]), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &sv[0] );
	siox_activity_set_attribute( sioxActivity, fileHandle, &sv[1] );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_socketpair)(domain, type, protocol, sv);

 	}
	return ret;
}

int accept(int  sockfd , struct sockaddr * addr , socklen_t * addrlen )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvaccept);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvaccept );
	siox_activity_set_attribute( sioxActivity, fileHandle, &sockfd );
	siox_activity_start(sioxActivity);
	ret = (__real_accept)(sockfd, addr, addrlen);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_accept)(sockfd, addr, addrlen);

 	}
	return ret;
}

int accept4(int  sockfd , struct sockaddr * addr , socklen_t * addrlen , int  flags )
{
	int ret;
	if( siox_monitoring_namespace_deactivated() && network_layer_initialized && siox_is_monitoring_enabled() ){
	assert(network_component);
	    	assert(cvaccept4);
	    	siox_activity * sioxActivity = siox_activity_begin( network_component, cvaccept4 );
	siox_activity_set_attribute( sioxActivity, fileHandle, &sockfd );
	siox_activity_start(sioxActivity);
	ret = (__real_accept4)(sockfd, addr, addrlen, flags);

	int errsv = errno;
		    if ( ret < 0 ){
                      siox_activity_report_error( sioxActivity, errsv );
                      siox_activity_stop(sioxActivity);
                      siox_activity_end(sioxActivity);
							 errno = errsv;
							 return ret;
                    }
	siox_activity_stop( sioxActivity );
	g_rw_lock_writer_lock(& lock_activityHashTable_network_int); 
    	g_hash_table_insert( activityHashTable_network_int, GINT_TO_POINTER(ret), siox_activity_get_ID(sioxActivity) );
    	g_rw_lock_writer_unlock(& lock_activityHashTable_network_int);
	siox_activity_set_attribute( sioxActivity, fileHandle, &ret );
	siox_activity_end( sioxActivity );
	}else{
		if(initialized_dlsym == 0) sioxSymbolInit();
	ret = (__real_accept4)(sockfd, addr, addrlen, flags);

 	}
	return ret;
}

