#include "multitasking_one.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

long long N = 10000000;
int NUM_TASKS = 2;

void sum_range(struct task_data *data)
{
    // Calculate sum for the task
    __int128_t _sum = 0;
    for (long long i = data->start; i < data->stop; i++)
    {
        _sum += i;
    }
    data->sum = _sum;
    return;
}

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
            // Close the read end of the pipe since child just needs to read
            close(fd[i][0]);
            // Calculate sum for the task using child's task_data object
            sum_range(&task_data_array[i]);
            // Write the sum of the child's task_data object to the pipe
            write(fd[i][1], &task_data_array[i].sum, sizeof(task_data_array[i].sum));
            // Close the write end of the pipe for this task since we are done with it
            close(fd[i][1]);
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

    // Return success
    printf("Range: 0 - %lld\nNum of Tasks: %d\n", N, NUM_TASKS);
    return 0;
}