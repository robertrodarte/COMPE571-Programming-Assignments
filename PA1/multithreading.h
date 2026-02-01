#ifndef MULTITHREADING_H
#define MULTITHREADING_H

// Used to pass parameters to pthread functions
struct thread_data {
    long long start;
    long long stop;
    __int128_t sum;
};

void *sum_range(void *arg);

#endif