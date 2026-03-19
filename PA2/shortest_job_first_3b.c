//------------------------------------------------------------------------------
// File:    shortest_job_first.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-03-07
// Brief:   Shortest Job First process scheduler with context switch timing
//------------------------------------------------------------------------------
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
#include <time.h>
#include <signal.h>
#include <sys/time.h>

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
struct Process
{
    pid_t pid;
    int workload;
    int id; /* original process number (1–4), preserved through sort */
};

//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//-----------------------------------------------------------------------------
static struct timespec cs_start, cs_end;
static double cs_total = 0.0;
static int cs_count = 0;
static int cs_running = 0;
static double rt_total = 0.0;
static int process_count = 0;

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
static void myfunction(int param);
static void short_sort(struct Process *procs, int n);
static void record_cs(void);
static void record_rt(double time);

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
    struct timespec start, end;

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
        At this point, all newly-created child processes are stopped, and ready for scheduling.
    *************************************************************************************************/

    /************************************************************************************************
        - Scheduling code starts here
        - SJF: pair each PID with its workload, sort ascending by workload,
          then run each process to completion in that order (non-preemptive).
    ************************************************************************************************/

    struct Process procs[4] = {
        {pid1, WORKLOAD1, 1},
        {pid2, WORKLOAD2, 2},
        {pid3, WORKLOAD3, 3},
        {pid4, WORKLOAD4, 4}};

    short_sort(procs, 4);

    struct timespec proc_start, proc_end;

    for (int i = 0; i < 4; i++)
    {
        record_cs();

        clock_gettime(CLOCK_MONOTONIC, &proc_start);
        kill(procs[i].pid, SIGCONT);
        waitpid(procs[i].pid, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &proc_end);

        clock_gettime(CLOCK_MONOTONIC, &cs_start);
        cs_running = 1;

        double exec_time = (proc_end.tv_sec - proc_start.tv_sec) + ((proc_end.tv_nsec - proc_start.tv_nsec) / 1E9);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double response_time = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        record_rt(response_time);
        printf("Process %d Time: %f\n", procs[i].id, response_time);
        printf("  Exec Time: %f s (workload %d)\n", exec_time, procs[i].workload);
    }

    /************************************************************************************************
        - Scheduling code ends here
    ************************************************************************************************/

    printf("Average Response Time: %f s\n", rt_total / process_count);
    printf("Context Switch Count: %d\n", cs_count);
    printf("Total Context Switch Time: %.3f ns\n", cs_total * 1E9);
    printf("Avg Context Switch Time: %.3f ns\n", (cs_total / cs_count) * 1E9);

    return 0;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
//=============================================================================
static void record_cs(void)
{
    if (cs_running)
    {
        clock_gettime(CLOCK_MONOTONIC, &cs_end);
        cs_total += (cs_end.tv_sec - cs_start.tv_sec) + ((cs_end.tv_nsec - cs_start.tv_nsec) / 1E9);
        cs_count++;
        cs_running = 0;
    }
}

//=============================================================================
static void record_rt(double time)
{
    rt_total += time;
    process_count++;
}

//=============================================================================
// Sort processes ascending by workload using bubble sort (Shortest Job First order)
static void short_sort(struct Process *procs, int n)
{
    int i, j;
    struct Process temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (procs[j].workload > procs[j + 1].workload)
            {
                temp = procs[j];
                procs[j] = procs[j + 1];
                procs[j + 1] = temp;
            }
        }
    }
}

//=============================================================================
/************************************************************************************************
                    DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
static void myfunction(int param)
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
/************************************************************************************************/

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__ |__) /__
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------