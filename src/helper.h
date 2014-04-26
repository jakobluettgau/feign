#ifndef HELPER_H_YDXEO6AL
#define HELPER_H_YDXEO6AL


// time related
struct timer {
	struct timespec start;
	struct timespec end;
	struct timespec delta;
};

void start_timer(struct timespec * start);
void stop_timer(struct timespec * start, struct timespec * end, int cycles);

void simple_nanosleep(long ns);

#endif /* end of include guard: HELPER_H_YDXEO6AL */

