//------------------------------------------------------------------------------
// File:    shortest_job_first.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-03-07
// Brief:   Shortest Job First
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /   |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#define _POSIX_C_SOURCE 200112L

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
struct Process
{
	pid_t pid;
	int workload;
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
void myfunction(int param);
void short_sort(struct Process *procs, int n);
int getAvg(struct timespec arr[], int n);

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
		  then run each process to completion in that order.
	************************************************************************************************/

	// procs[][0] = pid, procs[][1] = workload
	struct Process procs[4] = {
		{pid1, WORKLOAD1},
		{pid2, WORKLOAD2},
		{pid3, WORKLOAD3},
		{pid4, WORKLOAD4}};

	// Sort tasks by workload (shortest first)
	short_sort(procs, 4);

	// Execute processes in SJF order (non-preemptive: run each to completion)
	int status;

	struct timespec context_time[5];
	clock_gettime(CLOCK_MONOTONIC, &context_time[0]);

	for (int i = 0; i < 4; i++)
	{
		kill(procs[i].pid, SIGCONT);
		waitpid(procs[i].pid, &status, 0);
		clock_gettime(CLOCK_MONOTONIC, &context_time[i + 1]);
	}

	int average = getAvg(context_time, sizeof(context_time) / sizeof(context_time[0]) - 1);
	// Print timing results
	printf("SJF Context Switching Time Average: %d s\n", average);
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

/************************************************************************************************
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
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
/************************************************************************************************/

// Sort processes ascending by workload using bubble sort (Shortest Job First order)
// procs[][0] = pid, procs[][1] = workload
void short_sort(struct Process *procs, int n)
{
	int i, j;
	struct Process temp;

	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (procs[j].workload > procs[j + 1].workload)
			{
				// Swap entire Process — pid and workload move together automatically
				temp = procs[j];
				procs[j] = procs[j + 1];
				procs[j + 1] = temp;
			}
		}
	}
}

int getAvg(struct timespec arr[], int n)
{
	long delta = 0;

	long sum = 0;
	// Find the sum of all elements using a for loop
	for (int i = 0; i < n; i++)
	{
		if (i == 0)
		{
			continue; // Skip the first element since it has no previous element to compare to
		}
		delta = (arr[i].tv_sec - arr[i - 1].tv_sec) * 1e9 + (arr[i].tv_nsec - arr[i - 1].tv_nsec); // Calculate the difference between the current and previous element

		sum += delta;
	}
	// Return the average. Type cast sum to float to avoid integer division.
	return (long)sum / n;
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__ |__) /__
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------