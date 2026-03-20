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
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
struct Process
{
	pid_t pid;
	int workload;
	int id; /* original process number (1-4), preserved through sort */
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
static double rt_times[4] = {0.0};
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
	struct timespec start, end, total_end;
	double total1, total2, total3, total4;
	double total_exec_time, cs_ratio;

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

	/**************************************************************************
		Scheduling code starts here.
		SJF: pair each PID with its workload, sort ascending by workload,
		then run each process to completion in that order (non-preemptive).
	**************************************************************************/

	struct Process procs[4] = {
		{pid1, WORKLOAD1, 1},
		{pid2, WORKLOAD2, 2},
		{pid3, WORKLOAD3, 3},
		{pid4, WORKLOAD4, 4}};

	short_sort(procs, 4);

	// Process 1 (shortest job)
	kill(procs[0].pid, SIGCONT);
	waitpid(procs[0].pid, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
	total1 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
	record_rt(total1);
	printf("Process %d Time: %f s\n", procs[0].id, total1);
	clock_gettime(CLOCK_MONOTONIC, &cs_start);
	cs_running = 1;

	// Process 2
	record_cs();
	kill(procs[1].pid, SIGCONT);
	waitpid(procs[1].pid, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
	total2 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
	record_rt(total2);
	printf("Process %d Time: %f s\n", procs[1].id, total2);
	clock_gettime(CLOCK_MONOTONIC, &cs_start);
	cs_running = 1;

	// Process 3
	record_cs();
	kill(procs[2].pid, SIGCONT);
	waitpid(procs[2].pid, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
	total3 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
	record_rt(total3);
	printf("Process %d Time: %f s\n", procs[2].id, total3);
	clock_gettime(CLOCK_MONOTONIC, &cs_start);
	cs_running = 1;

	// Process 4 (longest job)
	record_cs();
	kill(procs[3].pid, SIGCONT);
	waitpid(procs[3].pid, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
	total4 = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1E9);
	record_rt(total4);
	printf("Process %d Time: %f s\n", procs[3].id, total4);

	/**************************************************************************
		Scheduling code ends here.
	**************************************************************************/

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
/******************************************************************************
				DO NOT CHANGE THE FUNCTION IMPLEMENTATION
******************************************************************************/
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
/******************************************************************************/

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------