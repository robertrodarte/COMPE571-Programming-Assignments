//------------------------------------------------------------------------------
// File:    mlfq.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-07-03
// Brief:   Implementing multi-level feedback queue scheduling algorithm
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

#define DEFAULT_QUANTUM 1000

#define DONE_QUEUE 0
#define RR_QUEUE 1
#define FCFS_QUEUE 2

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
static double rt_times[4] = {0.0};
static int process_count = 0;

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
static void myfunction(int param);
static void record_cs(void);
static void record_rt(double time);
static double calc_stddev(void);

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
    int ret;
    struct timespec start, end, total_end;
    double total1, total2, total3, total4;
    double total_exec_time, cs_ratio;
    int quantum = (argc > 1) ? atoi(argv[1]) : DEFAULT_QUANTUM;

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

    /**************************************************************************
        At this point, all newly-created child processes are stopped,
        and ready for scheduling.
    **************************************************************************/

    queue1 = queue2 = queue3 = queue4 = RR_QUEUE;

    // Exit loop if no queues should be round robin
    while (queue1 == RR_QUEUE || queue2 == RR_QUEUE || queue3 == RR_QUEUE || queue4 == RR_QUEUE)
    {
        if (queue1 == RR_QUEUE)
        {
            record_cs();
            kill(pid1, SIGCONT);
            usleep(quantum);
            kill(pid1, SIGSTOP);
            clock_gettime(CLOCK_MONOTONIC, &cs_start);
            cs_running = 1;

            ret = waitpid(pid1, NULL, WNOHANG);
            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total1 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                record_rt(total1);
                queue1 = DONE_QUEUE;
                printf("Process 1 Time: %f s\n", total1);
            }
            else
            {
                queue1 = FCFS_QUEUE;
            }
        }

        if (queue2 == RR_QUEUE)
        {
            record_cs();
            kill(pid2, SIGCONT);
            usleep(quantum);
            kill(pid2, SIGSTOP);
            clock_gettime(CLOCK_MONOTONIC, &cs_start);
            cs_running = 1;

            ret = waitpid(pid2, NULL, WNOHANG);
            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total2 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                record_rt(total2);
                queue2 = DONE_QUEUE;
                printf("Process 2 Time: %f s\n", total2);
            }
            else
            {
                queue2 = FCFS_QUEUE;
            }
        }

        if (queue3 == RR_QUEUE)
        {
            record_cs();
            kill(pid3, SIGCONT);
            usleep(quantum);
            kill(pid3, SIGSTOP);
            clock_gettime(CLOCK_MONOTONIC, &cs_start);
            cs_running = 1;

            ret = waitpid(pid3, NULL, WNOHANG);
            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total3 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                record_rt(total3);
                queue3 = DONE_QUEUE;
                printf("Process 3 Time: %f s\n", total3);
            }
            else
            {
                queue3 = FCFS_QUEUE;
            }
        }

        if (queue4 == RR_QUEUE)
        {
            record_cs();
            kill(pid4, SIGCONT);
            usleep(quantum);
            kill(pid4, SIGSTOP);
            clock_gettime(CLOCK_MONOTONIC, &cs_start);
            cs_running = 1;

            ret = waitpid(pid4, NULL, WNOHANG);
            if (ret > 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &end);
                total4 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
                record_rt(total4);
                queue4 = DONE_QUEUE;
                printf("Process 4 Time: %f s\n", total4);
            }
            else
            {
                queue4 = FCFS_QUEUE;
            }
        }
    }

    // If any processes were demoted to FCFS, handle them here
    if (queue1 == FCFS_QUEUE)
    {
        record_cs();
        kill(pid1, SIGCONT);
        waitpid(pid1, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total1 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        record_rt(total1);
        printf("Process 1 Time: %f s\n", total1);
        clock_gettime(CLOCK_MONOTONIC, &cs_start);
        cs_running = 1;
    }

    if (queue2 == FCFS_QUEUE)
    {
        record_cs();
        kill(pid2, SIGCONT);
        waitpid(pid2, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total2 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        record_rt(total2);
        printf("Process 2 Time: %f s\n", total2);
        clock_gettime(CLOCK_MONOTONIC, &cs_start);
        cs_running = 1;
    }

    if (queue3 == FCFS_QUEUE)
    {
        record_cs();
        kill(pid3, SIGCONT);
        waitpid(pid3, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total3 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        record_rt(total3);
        printf("Process 3 Time: %f s\n", total3);
        clock_gettime(CLOCK_MONOTONIC, &cs_start);
        cs_running = 1;
    }

    if (queue4 == FCFS_QUEUE)
    {
        record_cs();
        kill(pid4, SIGCONT);
        waitpid(pid4, NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total4 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
        record_rt(total4);
        printf("Process 4 Time: %f s\n", total4);
    }

    clock_gettime(CLOCK_MONOTONIC, &total_end);

    /* --- Summary metrics -------------------------------------------------- */
    total_exec_time = (total_end.tv_sec - start.tv_sec) +
                      ((total_end.tv_nsec - start.tv_nsec) / 1E9);
    cs_ratio = (cs_total / total_exec_time) * 100.0;

    printf("\n--- Scheduler Metrics ---\n");
    printf("Avg  Response Time:        %f s\n", rt_total / process_count);
    printf("Stddev Response Time:      %f s\n", calc_stddev());
    printf("Total Execution Time:      %.6f s\n", total_exec_time);
    printf("Context Switch Count:      %d\n", cs_count);
    printf("Total Context Switch Time: %.3f ns\n", cs_total * 1E9);
    printf("Avg  Context Switch Time:  %.3f ns\n", (cs_total / cs_count) * 1E9);
    printf("Context Switch Overhead:   %.4f%%\n", cs_ratio);

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
        cs_total += (cs_end.tv_sec - cs_start.tv_sec) +
                    ((cs_end.tv_nsec - cs_start.tv_nsec) / 1E9);
        cs_count++;
        cs_running = 0;
    }
}

//=============================================================================
static void record_rt(double time)
{
    rt_times[process_count] = time;
    rt_total += time;
    process_count++;
}

//=============================================================================
static double calc_stddev(void)
{
    int i;
    double mean = rt_total / process_count;
    double variance = 0.0;

    for (i = 0; i < process_count; i++)
    {
        double diff = rt_times[i] - mean;
        variance += diff * diff;
    }
    return __builtin_sqrt(variance / process_count);
}

//=============================================================================
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