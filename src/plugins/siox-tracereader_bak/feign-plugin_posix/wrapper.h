#ifndef WRAPPER_H_CJ0RYGIY
#define WRAPPER_H_CJ0RYGIY

#include "datatypes.h"

void dump_map();

int posix_open(void * data);
int posix_fopen(void * data);
int posix_read(void * data);
int posix_fread(void * data);
int posix_write(void * data);
int posix_close(void * data);
int posix_fclose(void * data);

int posix_lseek(void * data);

int posix_fclose(void * data);

#endif /* end of include guard: WRAPPER_H_CJ0RYGIY */
