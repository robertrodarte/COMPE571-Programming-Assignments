//------------------------------------------------------------------------------
// File:    round_robin.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-03-09
<<<<<<< Updated upstream
// Brief:   Short description of this module
=======
// Brief:   Round-robin process scheduler with context switch timing
>>>>>>> Stashed changes
//------------------------------------------------------------------------------
#define _POSIX_C_SOURCE 200112L

//------------------------------------------------------------------------------
//             __             __   ___  __
<<<<<<< Updated upstream
//     | |\ | /   |    |  | |  \ |__  /__
=======
//     | |\ | /  ` |    |  | |  \ |__  /__`
>>>>>>> Stashed changes
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
<<<<<<< Updated upstream
#include <string.h>
=======
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
=======
#define DEFAULT_QUANTUM 1000
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
void myfunction(int param);

void record_switch(double elapsed_us, double quantum_us,
				   double times[], int *count);

void print_switch_stats(const double times[], int count);
=======
static void myfunction(int param);
static void record_cs(void);
static void record_rt(double time);
>>>>>>> Stashed changes

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;
<<<<<<< Updated upstream

	// --- Context switch timing state ---
	struct timespec t_before, t_after;
	double switch_times[MAX_SWITCHES];
	int switch_count = 0;

	pid1 = fork();

	if (pid1 == 0)
	{

		myfunction(WORKLOAD1);

=======
	int ret;
	struct timespec start, end;
	double total1, total2, total3, total4;
	int quantum = (argc > 1) ? atoi(argv[1]) : DEFAULT_QUANTUM;

	clock_gettime(CLOCK_MONOTONIC, &start);

	pid1 = fork();
	if (pid1 == 0)
	{
		myfunction(WORKLOAD1);
>>>>>>> Stashed changes
		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();
<<<<<<< Updated upstream

	if (pid2 == 0)
	{

		myfunction(WORKLOAD2);

=======
	if (pid2 == 0)
	{
		myfunction(WORKLOAD2);
>>>>>>> Stashed changes
		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();
<<<<<<< Updated upstream

	if (pid3 == 0)
	{

		myfunction(WORKLOAD3);

=======
	if (pid3 == 0)
	{
		myfunction(WORKLOAD3);
>>>>>>> Stashed changes
		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();
<<<<<<< Updated upstream

	if (pid4 == 0)
	{

		myfunction(WORKLOAD4);

=======
	if (pid4 == 0)
	{
		myfunction(WORKLOAD4);
>>>>>>> Stashed changes
		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************
<<<<<<< Updated upstream
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
=======
		At this point, all newly-created child processes are stopped, and ready for scheduling.
>>>>>>> Stashed changes
	*************************************************************************************************/

	/************************************************************************************************
		- Scheduling code starts here
<<<<<<< Updated upstream
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods
		to be implemented.
=======
		- Round-robin: each process receives one quantum per cycle, in order.
		  Processes that have exited are skipped in subsequent cycles.
>>>>>>> Stashed changes
	************************************************************************************************/

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0)
		{
<<<<<<< Updated upstream
			clock_gettime(CLOCK_MONOTONIC, &t_before);
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &t_after);

			double elapsed_us = (t_after.tv_sec - t_before.tv_sec) * 1e6 + (t_after.tv_nsec - t_before.tv_nsec) / 1e3;
			record_switch(elapsed_us, QUANTUM1, switch_times, &switch_count);
=======
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
				running1 = 0;
				printf("Process 1 Time: %f\n", total1);
			}
>>>>>>> Stashed changes
		}

		if (running2 > 0)
		{
<<<<<<< Updated upstream
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
=======
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
				running2 = 0;
				printf("Process 2 Time: %f\n", total2);
			}
		}

		if (running3 > 0)
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
				running3 = 0;
				printf("Process 3 Time: %f\n", total3);
			}
		}

		if (running4 > 0)
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
				running4 = 0;
				printf("Process 4 Time: %f\n", total4);
			}
		}
>>>>>>> Stashed changes
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

<<<<<<< Updated upstream
	print_switch_stats(switch_times, switch_count);

	return 0;
}
=======
	printf("Average Response Time: %f s\n", rt_total / process_count);
	printf("Context Switch Count: %d\n", cs_count);
	printf("Total Context Switch Time: %.3f ns\n", cs_total * 1E9);
	printf("Avg Context Switch Time: %.3f ns\n", (cs_total / cs_count) * 1E9);

	return 0;
}

>>>>>>> Stashed changes
//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
<<<<<<< Updated upstream
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

=======
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
/************************************************************************************************
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
static void myfunction(int param)
{
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
//-----------------------------------------------------------------------------

/************************************************************************************************
		These DEFINE statements represent the workload size of each task and
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/
=======
//-----------------------------------------------------------------------------a
>>>>>>> Stashed changes
