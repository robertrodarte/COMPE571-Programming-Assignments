//------------------------------------------------------------------------------
// File:    multithreading.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-01-31
// Brief:   File that calculates the sum (non-inclusive) of variables 0-N.
//          Divides the workload evenly among different number of threads.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
long long N = 100000000;
int NUM_THREADS = 2;

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
struct thread_data
{
    long long start;
    long long stop;
    __int128_t sum;
};

//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
static void *sum_range(void *arg);

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
int main(int argc, char *argv[])
{
    // Handle command line inputs
    if (argc >= 3)
    {
        N = atoll(argv[1]);
        NUM_THREADS = atoll(argv[2]);
    }

    // Used to store thread ID's
    pthread_t threads[NUM_THREADS];
    // Used to hold data for each thread
    struct thread_data thread_data_array[NUM_THREADS];
    // Calculates chunks to sum over
    long long chunk_size = N / NUM_THREADS;
    // Initialize total sum
    __int128_t total_sum = 0;

    // Create all the threads
    for (long long i = 0; i < NUM_THREADS; i++)
    {
        // Initialize thread info
        thread_data_array[i].start = i * chunk_size;
        thread_data_array[i].sum = 0;

        // Due to indexing, don't skip last N value
        if (i == NUM_THREADS - 1)
        {
            thread_data_array[i].stop = N;
        }
        else
        {
            thread_data_array[i].stop = (i + 1) * chunk_size;
        }

        // Spawn a new thread with info
        // sum_range will be the function the thread executes
        pthread_create(&threads[i], NULL, sum_range, &thread_data_array[i]);
    }

    // Wait for all threads to finish
    for (long long i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Store output of all threads in total sum
    for (long long i = 0; i < NUM_THREADS; i++)
    {
        total_sum += thread_data_array[i].sum;
    }

    // Return success
    printf("Range: 0 - %lld\nNum of Threads: %d\n", N, NUM_THREADS);
    return 0;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
static void *sum_range(void *arg)
{
    // Initialize
    __int128_t _sum = 0;
    // Cast void* back to struct thread_data*
    struct thread_data *data = (struct thread_data *)arg;

    // Loop from start to end, calculating sums
    for (long long i = data->start; i < data->stop; i++)
    {
        _sum += i;
    }

    // Store result in structs sum field
    data->sum = _sum;
    // Nothing to return
    return NULL;
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------