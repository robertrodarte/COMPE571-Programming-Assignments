//------------------------------------------------------------------------------
// File:    fcfs.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-07-03
// Brief:   Implementing first come first serve scheduling algorithm
//------------------------------------------------------------------------------
// Used for timing
#define _POSIX_C_SOURCE 200112L

//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------

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
void myfunction(int param);

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
int main(int argc, char const *argv[])
{
    pid_t pid1, pid2, pid3, pid4;
    int queue1, queue2, queue3, queue4;
    int status1, status2, status3, status4;
    int ret;
    struct timespec start, end;
    double total1, total2, total3, total4;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pid1 = fork();

    if (pid1 == 0)
    {

        myfunction(WORKLOAD1);

        exit(0);
    }
    kill(pid1, SIGSTOP);

    pid2 = fork();

    if (pid2 == 0)
    {

        myfunction(WORKLOAD2);

        exit(0);
    }
    kill(pid2, SIGSTOP);

    pid3 = fork();

    if (pid3 == 0)
    {

        myfunction(WORKLOAD3);

        exit(0);
    }
    kill(pid3, SIGSTOP);

    pid4 = fork();

    if (pid4 == 0)
    {

        myfunction(WORKLOAD4);

        exit(0);
    }
    kill(pid4, SIGSTOP);

    /************************************************************************************************
        At this point, all  newly-created child processes are stopped, and ready for scheduling.
    *************************************************************************************************/

    /************************************************************************************************
        - Scheduling code starts here
        - Below is a sample schedule. (which scheduling algorithm is this?)
        - For the assignment purposes, you have to replace this part with the other scheduling methods
        to be implemented.
    ************************************************************************************************/
    status1 = 1;
    status2 = 1;
    status3 = 1;
    status4 = 1;

    queue1 = 1;
    queue2 = 1;
    queue3 = 1;
    queue4 = 1;

    // Exit loop if no queues should be round robin
    while (queue1 == 1 || queue2 == 1 || queue3 == 1 || queue4 == 1)
    {
        if (queue1 == 1)
        {
            // Start signal
            kill(pid1, SIGCONT);
            // Run for specific duration
            usleep(QUANTUM1);
            // Pause process
            kill(pid1, SIGSTOP);

            // Check process status (Done: status == 0)
            ret = waitpid(pid1, &status1, WNOHANG);
            if (ret > 0)
            {
                // If ret > 0, process is done running
                clock_gettime(CLOCK_MONOTONIC, &end);
                total1 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                queue1 = 0;
                printf("Process 1 Time: %f\n", total1);
            }
            else
            {
                // Otherwise, not finished so go to fcfs
                queue1 = 2;
            }
        }
        if (queue2 == 1)
        {
            kill(pid2, SIGCONT);
            usleep(QUANTUM2);
            kill(pid2, SIGSTOP);
            ret = waitpid(pid2, &status2, WNOHANG);

            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total2 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                queue2 = 0;
                printf("Process 2 Time: %f\n", total2);
            }
            else
            {
                queue2 = 2;
            }
        }
        if (queue3 == 1)
        {
            kill(pid3, SIGCONT);
            usleep(QUANTUM3);
            kill(pid3, SIGSTOP);
            ret = waitpid(pid3, &status3, WNOHANG);

            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total3 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                queue3 = 0;
                printf("Process 3 Time: %f\n", total3);
            }
            else
            {
                queue3 = 2;
            }
        }
        if (queue4 == 1)
        {
            kill(pid4, SIGCONT);
            usleep(QUANTUM4);
            kill(pid4, SIGSTOP);
            ret = waitpid(pid4, &status4, WNOHANG);

            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total4 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                queue4 = 0;
                printf("Process 4 Time: %f\n", total4);
            }
            else
            {
                queue4 = 2;
            }
        }
    }

    // If any processes were demoted to fcfs, handle it here
    if (queue1 == 2)
    {
        kill(pid1, SIGCONT);
        waitpid(pid1, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total1 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        printf("Process 1 Time: %f\n", total1);
    }
    if (queue2 == 2)
    {
        kill(pid2, SIGCONT);
        waitpid(pid2, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total2 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        printf("Process 2 Time: %f\n", total2);
    }
    if (queue3 == 2)
    {
        kill(pid3, SIGCONT);
        waitpid(pid3, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total3 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        printf("Process 3 Time: %f\n", total3);
    }
    if (queue4 == 2)
    {
        kill(pid4, SIGCONT);
        waitpid(pid4, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total4 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        printf("Process 4 Time: %f\n", total4);
    }

    printf("Average Response Time: %f\n", (total1 + total2 + total3 + total4) / 4.0);

    /************************************************************************************************
        - Scheduling code ends here
    ************************************************************************************************/

    return 0;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
//=============================================================================
void myfunction(int param)
{

    int i = 2;
    int j, k;

    while (i < param)
    {
        k = i;
        for (j = 2; j <= k; j++)
        {
            if (k % j == 0)
            {
                k = k / j;
                j--;
                if (k == 1)
                {
                    break;
                }
            }
        }
        i++;
    }
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------