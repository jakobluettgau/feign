#ifndef REPLAY_H_RBZA8ICV
#define REPLAY_H_RBZA8ICV
#include "common.h"

int preload();
int replay();

void reset_buffer_flags();

void replay_manager_print_stats();
void reset_replay_stats();
void print_replay_buffer();

void set_precreation();
void unset_precreation();



#endif /* end of include guard: REPLAY_H_RBZA8ICV */
