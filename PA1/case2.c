
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "multithreading.h"


 __int128 workload(__int128 start, __int128 stop) {
    __int128 sum = 0;
    for (__int128 i = start; i < stop; ++i) {
        sum += i;
    }
    return sum;
}



 void* thread_fn(void* arg) {
    thread_arg_t* a = (thread_arg_t*)arg;
    a->partial = workload(a->start, a->stop);
    return NULL;
}

int main(int argc, char *argv[]) {
    
    __int128 N = atoll(argv[1]);
    int NUM_THREADS = atoi(argv[2]);

    if (N % NUM_THREADS != 0) {
    fprintf(stderr, "N must be divisible by num_threads\n");
    return 1;
    }



    __int128 chunk = N / (__int128)NUM_THREADS;

    pthread_t* threads = malloc(sizeof(pthread_t) * NUM_THREADS);
    thread_arg_t* args = malloc(sizeof(thread_arg_t) * NUM_THREADS);

    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].start = (__int128)t * chunk;
        args[t].stop  = (__int128)(t + 1) * chunk;
        args[t].partial = 0;
        args[t].thread_id = t;

        int rc = pthread_create(&threads[t], NULL, thread_fn, &args[t]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed (t=%d, rc=%d)\n", t, rc);
            exit(1);
        }
    }

    __int128 total = 0;
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
        total += args[t].partial;
    }

    // print total using helper (not shown here)
    free(args);
    free(threads);
    return 0;
}

