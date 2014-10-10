#include  <stdlib.h>
#include  <stdio.h>
#include  <stdarg.h>
#include  <string>
#include  <iostream>
#include  <map>
#include  <stdint.h>
#include  <feign.h>
#include  "posix/datatypes.h"

struct fileinfo
{
	std::string filepath;	// path to file, not really needed for our purpose
	int filehandle;
	int create_size;		// the minium file size when precreated
	int size;				// running file size
	int flags;				// file open flags r? r+? w? rw? a?
	int mode;				// file permissions, is ignored here
	int pos;				// file handle position
	int precreate;			// flag that this needs to be precreated
};


// map filepath to handle
std::map<int, fileinfo> fd2info;		    // sizes not populated, lookup in global
std::map<std::string, fileinfo> file2ginfo; // global fileinfo, keeps track of size, mode and flags not cared for




void dump_fd2info() {
	printf("dump_fd2info()\n");
	for (auto it=fd2info.begin(); it!=fd2info.end(); ++it) {
		std::cout << it->first << ".filepath => " << it->second.filepath << '\n';
		std::cout << it->first << ".size => " << it->second.size << '\n';
		std::cout << it->first << ".create_size => " << it->second.create_size << '\n';
		std::cout << it->first << ".flags => " << it->second.flags << '\n';
		std::cout << it->first << ".mode => " << it->second.mode << '\n';
		std::cout << it->first << ".pos => " << it->second.pos << '\n';
		std::cout << it->first << ".precreate => " << it->second.precreate << '\n';
	}
}


void dump_file2ginfo() {
	printf("dump_file2ginfo()\n");
	for (auto it=file2ginfo.begin(); it!=file2ginfo.end(); ++it) {
		std::cout << it->first << ".filepath => " << it->second.filepath << '\n';
		std::cout << it->first << ".size => " << it->second.size << '\n';
		std::cout << it->first << ".create_size => " << it->second.create_size << '\n';
		std::cout << it->first << ".flags => " << it->second.flags << '\n';
		std::cout << it->first << ".mode => " << it->second.mode << '\n';
		std::cout << it->first << ".pos => " << it->second.pos << '\n';
		std::cout << it->first << ".precreate => " << it->second.precreate << '\n';
	}
}





// provide some information about the plugin
////////////////////////////////////////////
Plugin plugin = {
	.name = "posix-precreator",
	.version = NULL,
	.intents = FEIGN_PRECREATOR,
};

int layer_id = 13;

Plugin * init() {
    // give a sign of life
	printf("Hello from %s\n", plugin.name);

	// announce features and request further action
	return &plugin;
}


//Activity * replay(Activity * activity) 
Activity * precreate_check(Activity * activity) 
{

	dump_fd2info();
	dump_file2ginfo();

	FEIGN_LOG(3,"precreate-check");

	if ( activity->layer == layer_id ) {
		posix_activity * sub_activity = (posix_activity*)activity->data;

		void * data = sub_activity->data;

        // a simple way to produce action
		switch ( sub_activity->type ) {

			case POSIX_open:
				{
					posix_open_data * d = (posix_open_data*) data;
					printf( "open: pathname: %s, flags: %d, translatedFlags=%d, ret: %d\n", d->pathname, d->flags, d->translatedFlags, d->ret);
				

					if ( fd2info.count(d->ret) )  {
						// trace invalid
						DEBUG("Invalid tracedata, file handle collision!");
					} else {
						DEBUG("Add to precreate!!");

						fileinfo fi = { .filepath = d->pathname };
						fi.precreate = 1;
						fd2info[d->ret] = fi;
						file2ginfo[d->pathname] = fi;

					}

				}
				//wrapped_open(sub_activity->data);
				break;

			case POSIX_creat:
				//wrapped_creat(sub_activity->data);
				break;

			case POSIX_open64:
				//wrapped_open64(sub_activity->data);
				break;

			case POSIX_creat64:
				//wrapped_creat64(sub_activity->data);
				break;

			case POSIX_close:
				//wrapped_close(sub_activity->data);
				break;

			case POSIX_dup:
				//wrapped_dup(sub_activity->data);
				break;

			case POSIX_dup2:
				//wrapped_dup2(sub_activity->data);
				break;

			case POSIX_dup3:
				//wrapped_dup3(sub_activity->data);
				break;

			case POSIX_sendfile:
				//wrapped_sendfile(sub_activity->data);
				break;

			case POSIX_write:
				//wrapped_write(sub_activity->data);
				break;

			case POSIX_read:
				{
					posix_read_data * d = (posix_read_data*) data;

					feign_log(1 ,"read: fd: %d~>%d, count: %d, ret: %d\n", d->fd, "not mapped", d->count, d->ret);

					file2ginfo[fd2info[d->fd].filepath].create_size += d->count;

				}
				//wrapped_read(sub_activity->data);
				break;

			case POSIX_writev:
				//wrapped_writev(sub_activity->data);
				break;

			case POSIX_readv:
				//wrapped_readv(sub_activity->data);
				break;

			case POSIX_pwrite:
				//wrapped_pwrite(sub_activity->data);
				break;

			case POSIX_pread:
				//wrapped_pread(sub_activity->data);
				break;

			case POSIX_pwrite64:
				//wrapped_pwrite64(sub_activity->data);
				break;

			case POSIX_pread64:
				//wrapped_pread64(sub_activity->data);
				break;

			case POSIX_pwritev:
				//wrapped_pwritev(sub_activity->data);
				break;

			case POSIX_preadv:
				//wrapped_preadv(sub_activity->data);
				break;

			case POSIX_pwritev64:
				//wrapped_pwritev64(sub_activity->data);
				break;

			case POSIX_preadv64:
				//wrapped_preadv64(sub_activity->data);
				break;

			case POSIX_sync:
				//wrapped_sync(sub_activity->data);
				break;

			case POSIX_fsync:
				//wrapped_fsync(sub_activity->data);
				break;

			case POSIX_fdatasync:
				//wrapped_fdatasync(sub_activity->data);
				break;

			case POSIX_lseek:
				//wrapped_lseek(sub_activity->data);
				break;

			case POSIX_posix_fadvise:
				//wrapped_posix_fadvise(sub_activity->data);
				break;

			case POSIX_remove:
				//wrapped_remove(sub_activity->data);
				break;

			case POSIX_rename:
				//wrapped_rename(sub_activity->data);
				break;

			case POSIX___xstat64:
				//wrapped___xstat64(sub_activity->data);
				break;

			case POSIX___lxstat64:
				//wrapped___lxstat64(sub_activity->data);
				break;

			case POSIX___fxstat64:
				//wrapped___fxstat64(sub_activity->data);
				break;

			case POSIX___fxstat:
				//wrapped___fxstat(sub_activity->data);
				break;

			case POSIX_mmap:
				//wrapped_mmap(sub_activity->data);
				break;

			case POSIX_mmap64:
				//wrapped_mmap64(sub_activity->data);
				break;

			case POSIX_fopen:
				//wrapped_fopen(sub_activity->data);
				break;

			case POSIX_fopen64:
				//wrapped_fopen64(sub_activity->data);
				break;

			case POSIX_fdopen:
				//wrapped_fdopen(sub_activity->data);
				break;

			case POSIX_fileno:
				//wrapped_fileno(sub_activity->data);
				break;

			case POSIX_freopen:
				//wrapped_freopen(sub_activity->data);
				break;

			case POSIX_tmpfile:
				//wrapped_tmpfile(sub_activity->data);
				break;

			case POSIX_fclose:
				//wrapped_fclose(sub_activity->data);
				break;

			case POSIX_fflush:
				//wrapped_fflush(sub_activity->data);
				break;

			case POSIX_fgetc:
				//wrapped_fgetc(sub_activity->data);
				break;

			case POSIX_getc:
				//wrapped_getc(sub_activity->data);
				break;

			case POSIX_fputc:
				//wrapped_fputc(sub_activity->data);
				break;

			case POSIX_putc:
				//wrapped_putc(sub_activity->data);
				break;

			case POSIX_fgets:
				//wrapped_fgets(sub_activity->data);
				break;

			case POSIX_fputs:
				//wrapped_fputs(sub_activity->data);
				break;

			case POSIX_fread:
				//wrapped_fread(sub_activity->data);
				break;

			case POSIX_fwrite:
				//wrapped_fwrite(sub_activity->data);
				break;

			case POSIX_fseeko:
				//wrapped_fseeko(sub_activity->data);
				break;

			case POSIX_fseek:
				//wrapped_fseek(sub_activity->data);
				break;

			case POSIX_setbuf:
				//wrapped_setbuf(sub_activity->data);
				break;

			case POSIX_setvbuf:
				//wrapped_setvbuf(sub_activity->data);
				break;

			case POSIX_unlink:
				//wrapped_unlink(sub_activity->data);
				break;

			case POSIX_vfprintf:
				//wrapped_vfprintf(sub_activity->data);
				break;

			case POSIX_vfscanf:
				//wrapped_vfscanf(sub_activity->data);
				break;

			case POSIX_fscanf:
				//wrapped_fscanf(sub_activity->data);
				break;

			case POSIX_fprintf:
				//wrapped_fprintf(sub_activity->data);
				break;

			case POSIX_aio_read:
				//wrapped_aio_read(sub_activity->data);
				break;

			case POSIX_aio_write:
				//wrapped_aio_write(sub_activity->data);
				break;

			case POSIX_lio_listio:
				//wrapped_lio_listio(sub_activity->data);
				break;

			case POSIX_aio_suspend:
				//wrapped_aio_suspend(sub_activity->data);
				break;

			case POSIX_aio_cancel:
				//wrapped_aio_cancel(sub_activity->data);
				break;

			case POSIX_fork:
				//wrapped_fork(sub_activity->data);
				break;

			case POSIX_lockf:
				//wrapped_lockf(sub_activity->data);
				break;

			case POSIX_flock:
				//wrapped_flock(sub_activity->data);
				break;

			case POSIX_socket:
				//wrapped_socket(sub_activity->data);
				break;

			case POSIX_setsockopt:
				//wrapped_setsockopt(sub_activity->data);
				break;

			case POSIX_pipe:
				//wrapped_pipe(sub_activity->data);
				break;

			case POSIX_pipe2:
				//wrapped_pipe2(sub_activity->data);
				break;

			case POSIX_socketpair:
				//wrapped_socketpair(sub_activity->data);
				break;

			case POSIX_accept:
				//wrapped_accept(sub_activity->data);
				break;

			case POSIX_accept4:
				//wrapped_accept4(sub_activity->data);
				break;


            default:
                FEIGN_LOG(8,"unknown type");
        }
    } else {
        FEIGN_LOG(5, "!");
    }

    return NULL;
} // end replay()






Activity * precreate(Activity * activity) {
	
	FEIGN_LOG(3,"precreate()");

	for (auto it=file2ginfo.begin(); it!=file2ginfo.end(); ++it) {

		if ( it->second.precreate ) {
			std::cout << it->first << " - create_size=" << it->second.create_size << '\n';

			if ( it->second.create_size > 0 )
				feign_precreate_file(it->first.c_str(), it->second.create_size);
		}
	}


	return NULL;
}
