//------------------------------------------------------------------------------
// File:    multitasking_two.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-01-31
// Brief:   File that calculates the sum (non-inclusive) of variables 0-N.
//          Divides the workload evenly using multitasking (concurrent).
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
#include <stdlib.h>
#include <string.h>
#include <time.h>


//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__/ |___ |___ .__/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
static __int128_t sum_range(long long lo, long long hi);
static void print_int128(__int128_t n);


//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__/ |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
int main(int argc, char *argv[])
{
   // Initialize time variables
   struct timespec t_start, t_end;
   double total_work_time;


   // Check if running as child process `--child <lo> <hi>`
   if (argc == 4 && strcmp(argv[1], "--child") == 0)
   {
       long long lo = atoll(argv[2]);
       long long hi = atoll(argv[3]);


       __int128_t partial_sum = sum_range(lo, hi);


       // Split __int128_t into two 64-bit halves to pass through pipe
       unsigned long long lo_half = (unsigned long long)(partial_sum & 0xFFFFFFFFFFFFFFFFULL);
       unsigned long long hi_half = (unsigned long long)((__uint128_t)partial_sum >> 64);


       // Send both halves to parent via stdout pipe
       printf("%llu %llu\n", hi_half, lo_half);
       return 0;
   }


   // Handle command line inputs for parent process (assume perfect inputs)
   long long N = atoll(argv[1]);
   int NUM_TASKS = atoi(argv[2]);


   // Allocate array of FILE pointers — one per child process
   FILE **fps = malloc(NUM_TASKS * sizeof(FILE *));


   // Get start timestamp before spawning any children
   clock_gettime(CLOCK_MONOTONIC, &t_start);


   // --- Spawn ALL children concurrently ---
   for (int i = 0; i < NUM_TASKS; i++)
   {
       long long lo = (N * i) / NUM_TASKS;
       long long hi = (N * (i + 1)) / NUM_TASKS;


       char cmd[256];
       snprintf(cmd, sizeof(cmd), "%s --child %lld %lld", argv[0], lo, hi);


       fps[i] = popen(cmd, "r");
   }


   // --- Collect results from all children ---
   __int128_t total_sum = 0;
   for (int i = 0; i < NUM_TASKS; i++)
   {
       long long lo = (N * i) / NUM_TASKS;
       long long hi = (N * (i + 1)) / NUM_TASKS;


       // Read the two 64-bit halves sent by the child via fscanf (IPC through pipe)
       unsigned long long hi_half = 0, lo_half = 0;
       fscanf(fps[i], "%llu %llu", &hi_half, &lo_half);


       pclose(fps[i]);


       // Reconstruct the __int128_t partial sum from the two halves
       __int128_t partial = ((__int128_t)hi_half << 64) | (__int128_t)lo_half;


       printf("[parent] chunk %d/%d range [%lld, %lld) partial=",
              i + 1, NUM_TASKS, lo, hi);
       print_int128(partial);
       printf("\n");


       total_sum += partial;
   }


   // Get end timestamp after all children have finished
   clock_gettime(CLOCK_MONOTONIC, &t_end);


   total_work_time = ((t_end.tv_sec - t_start.tv_sec) * 1e9) + (t_end.tv_nsec - t_start.tv_nsec);


   // Return success
   printf("Total Sum: ");
   print_int128(total_sum);
   printf("\n");
   printf("Range: 0 - %lld\nNum of Tasks: %d\nTotal Time: %f (ns) or %f (s)\n",
          N, NUM_TASKS, total_work_time, total_work_time / 1e9);


   free(fps);
   return 0;
}


//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
//=============================================================================


static __int128_t sum_range(long long lo, long long hi)
{
   // Calculate sum for the task
   __uint128_t sum = 0;
   for (long long i = lo; i < hi; i++)
   {
       sum += i;
   }
   return sum;
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
