//------------------------------------------------------------------------------
// File:    round_robin.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-03-09
// Brief:   Short description of this module
//------------------------------------------------------------------------------
#define _POSIX_C_SOURCE 200112L

//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /   |    |  | |  \ |__  /__
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

#define MAX_SWITCHES 2048

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

void record_switch(double elapsed_us, double quantum_us,
				   double times[], int *count);

void print_switch_stats(const double times[], int count);

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
	int running1, running2, running3, running4;

	// --- Context switch timing state ---
	struct timespec t_before, t_after;
	double switch_times[MAX_SWITCHES];
	int switch_count = 0;

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

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0)
		{
			clock_gettime(CLOCK_MONOTONIC, &t_before);
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &t_after);

			double elapsed_us = (t_after.tv_sec - t_before.tv_sec) * 1e6 + (t_after.tv_nsec - t_before.tv_nsec) / 1e3;
			record_switch(elapsed_us, QUANTUM1, switch_times, &switch_count);
		}

		if (running2 > 0)
		{
			clock_gettime(CLOCK_MONOTONIC, &t_before);
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &t_after);

			double elapsed_us = (t_after.tv_sec - t_before.tv_sec) * 1e6 + (t_after.tv_nsec - t_before.tv_nsec) / 1e3;
			record_switch(elapsed_us, QUANTUM2, switch_times, &switch_count);
		}
		if (running3 > 0)
		{
			clock_gettime(CLOCK_MONOTONIC, &t_before);
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &t_after);

			double elapsed_us = (t_after.tv_sec - t_before.tv_sec) * 1e6 + (t_after.tv_nsec - t_before.tv_nsec) / 1e3;
			record_switch(elapsed_us, QUANTUM3, switch_times, &switch_count);
		}
		if (running4 > 0)
		{
			clock_gettime(CLOCK_MONOTONIC, &t_before);
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &t_after);

			double elapsed_us = (t_after.tv_sec - t_before.tv_sec) * 1e6 + (t_after.tv_nsec - t_before.tv_nsec) / 1e3;
			record_switch(elapsed_us, QUANTUM4, switch_times, &switch_count);
		}
		waitpid(pid1, &running1, WNOHANG);
		waitpid(pid2, &running2, WNOHANG);
		waitpid(pid3, &running3, WNOHANG);
		waitpid(pid4, &running4, WNOHANG);
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	print_switch_stats(switch_times, switch_count);

	return 0;
}
//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
void record_switch(double elapsed_us, double quantum_us,
				   double times[], int *count)
{
	if (*count >= MAX_SWITCHES)
		return;

	double overhead_us = elapsed_us - quantum_us;
	if (overhead_us < 0.0)
		overhead_us = 0.0;

	times[(*count)++] = overhead_us;
}

void print_switch_stats(const double times[], int count)
{

	double total = 0.0;
	double min_val = times[0];
	double max_val = times[0];

	for (int i = 0; i < count; i++)
	{
		total += times[i];
		if (times[i] < min_val)
			min_val = times[i];
		if (times[i] > max_val)
			max_val = times[i];
	}

	printf("\n--- Context Switch Statistics ---\n");
	printf("Samples  : %d\n", count);
	printf("Avg      : %.3f us\n", total / count);
	printf("Min      : %.3f us\n", min_val);
	printf("Max      : %.3f us\n", max_val);
	printf("---------------------------------\n");
}

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

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__ |__) /__
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------

/************************************************************************************************
		These DEFINE statements represent the workload size of each task and
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/
