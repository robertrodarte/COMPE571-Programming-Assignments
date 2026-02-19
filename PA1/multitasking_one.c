//------------------------------------------------------------------------------
// File:    multitasking_one.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-01-31
// Brief:   File that calculates the sum (non-inclusive) of variables 0-N.
//          Divides the workload evenly using multitasking.
//------------------------------------------------------------------------------
// Used for timing
#define _POSIX_C_SOURCE 199309L

//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
long long N = 100000000;
int NUM_TASKS = 2;

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
struct task_data
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
static void sum_range(struct task_data *data);
static void print_int128(__int128_t n);

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
        NUM_TASKS = atoll(argv[2]);
    }

    // Keeps track of pipes
    int fd[NUM_TASKS][2];
    // Keeps track of pids
    int pids[NUM_TASKS];
    // Keeps track of task data
    struct task_data task_data_array[NUM_TASKS];

    // Stores sum
    __int128_t total_sum = 0;
    // Calculates chunks to sum over
    long long chunk_size = N / NUM_TASKS;

    // Initialize time variables
    struct timespec t_start, t_end;
    double total_work_time;

    // Get timestamp
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Create pipes
    for (int i = 0; i < NUM_TASKS; i++)
    {
        // Initialize start and stop for task
        task_data_array[i].start = i * chunk_size;
        task_data_array[i].sum = 0;

        // Due to indexing, don't skip last N value
        if (i == NUM_TASKS - 1)
        {
            task_data_array[i].stop = N;
        }
        else
        {
            task_data_array[i].stop = (i + 1) * chunk_size;
        }

        // Create a pipe for the task
        pipe(fd[i]);

        // Create the child
        pids[i] = fork();

        // CHILD BEGINS EXECUTING CODE HERE ALONG WITH PARENT PROCESS

        // Determine if child(pid == 0) or parent process
        if (0 == pids[i])
        {
            // Close the read end of the pipe since child just needs to write
            close(fd[i][0]);
            // Close stdout so we can assign it with dup
            close(STDOUT_FILENO);
            // Assign stdout to pipe write
            dup(fd[i][1]);
            // Close the write end of the pipe for this task since we are done with it
            close(fd[i][1]);
            // Calculate sum for the task using child's task_data object
            sum_range(&task_data_array[i]);
            // Write the sum of the child's task_data object to the pipe using stdout (dup)
            write(STDOUT_FILENO, &task_data_array[i].sum, sizeof(task_data_array[i].sum));
            // Kill the child process so it doesn't continue running
            _exit(0);
        }
        else
        {
            // Close the parent write end since we don't use it
            close(fd[i][1]);
        }
    }

    // THE PARENT PROCESS WILL REACH HERE BEFORE CHILD PROCESSES ARE DONE, SO WE NEED TO HANDLE THAT BY WAITING

    for (int i = 0; i < NUM_TASKS; i++)
    {
        // Wait for child task to end
        waitpid(pids[i], NULL, 0);
        // Read the child's data from the pipe into the parents task_data object
        read(fd[i][0], &task_data_array[i].sum, sizeof(task_data_array[i].sum));
        // Store the sum info into total
        total_sum += task_data_array[i].sum;
        // Close the pipe
        close(fd[i][0]);
    }

    // Grab timestamp
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    // Calculate work time
    total_work_time = ((t_end.tv_sec - t_start.tv_sec) * 1e9) + (t_end.tv_nsec - t_start.tv_nsec);

    // Return success
    printf("Total Sum: ");
    print_int128(total_sum);
    printf("\n");
    printf("Range: 0 - %lld\nNum of Tasks: %d\nTotal Time: %f (ns) or %f (s)\n", N, NUM_TASKS, total_work_time, total_work_time / 1e9);
    return 0;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
//=============================================================================
static void sum_range(struct task_data *data)
{
    // Calculate sum for the task
    __int128_t sum = 0;
    for (long long i = data->start; i < data->stop; i++)
    {
        sum += i;
    }
    data->sum = sum;
    return;
}

//=============================================================================
static void print_int128(__int128_t n)
{
    // If negative print a negative sign
    if (n < 0)
    {
        printf("-");
        n = -n;
    }

    // If n is greater than 9
    if (n > 9)
        // Keep calling recursively to print each digit of the large sum
        print_int128(n / 10);

    // Print the digit as a char using an ASCII conversion
    printf("%c", (char)('0' + n % 10));
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------