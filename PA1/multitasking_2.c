//------------------------------------------------------------------------------
// File:    multitasking_two.c
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-01-31
// Brief:   File that calculates the sum (non-inclusive) of variables 0-N.
//          Divides the workload evenly using multitasking.
//------------------------------------------------------------------------------

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
#include <inttypes.h>
#include <string.h>

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
long long N = 10000000;
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
//      \/  /~~\ |  \ | /~~\ |__/ |___ |___ .__/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
static __int128_t sum_range(long long lo, long long h)
{
    // Calculate sum for the task
    __uint128_t sum = 0;
    for (long long i = lo; i < h; i++)
    {
        sum += i;
    }
    return sum;
}

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__/ |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
int main(int argc, char *argv[])
{

    // Check if running as child process `--child <lo> <hi>`
    if (argc == 4 && strcmp(argv[1], "--child") == 0)
    {
        long long lo = atoll(argv[2]);
        long long hi = atoll(argv[3]);

        __int128_t partial_sum = sum_range(lo, hi);

        // Print Child Partial Sum to stderr for debugging, and to stdout for parent to read
        fprintf(stderr, "[child] sum [%lld, %lld) = %llu\n",
                lo, hi, (unsigned long long)partial_sum);

        printf("%llu\n", (unsigned long long)partial_sum);
        return 0;
    }

    // Handle command line inputs for parent process
    if (argc != 3)
    {
        printf("Usage: %s <N> <NUM_TASKS>\n", argv[0]);
        exit(1);
    }

    // Use different names or just reuse globals
    long long N = atoll(argv[1]);
    int NUM_TASKS = atoi(argv[2]);

    if (N < 2)
    {
        fprintf(stderr, "N must be >= 2 (since range is [1, N)).\n");
        return 1;
    }
    if (NUM_TASKS < 1)
    {
        fprintf(stderr, "NUM_TASKS must be >= 1.\n");
        return 1;
    }
    if (NUM_TASKS > (N - 1))
    {
        NUM_TASKS = N - 1;
        fprintf(stderr, "Note: Capping NUM_TASKS to %d (numbers in range).\n", NUM_TASKS);
    }

    __int128_t total_sum = 0;

    // Loop in the parent: spawn one child per chunk, read its partial sum, add up
    for (int i = 0; i < NUM_TASKS; i++)
    {
        long long lo = (N * i) / NUM_TASKS;
        long long hi = (N * (i + 1)) / NUM_TASKS;

        // Build command to run SAME program in child mode for [lo, hi)
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s --child %lld %lld", argv[0], lo, hi);

        FILE *fp = popen(cmd, "r");
        if (!fp)
        {
            perror("popen");
            return 1;
        }

        unsigned long long partial_sum = 0;
        if (fscanf(fp, "%llu", &partial_sum) != 1)
        {
            fprintf(stderr, "Failed reading child %d output for [%lld, %lld).\n",
                    i, lo, hi);
            pclose(fp);
            return 1;
        }

        pclose(fp);

        printf("[parent] chunk %d/%d range [%lld, %lld) partial=%llu\n",
               i + 1, NUM_TASKS, lo, hi, partial_sum);

        total_sum += partial_sum;
    }

    printf("[parent] total = %llu\n", (unsigned long long)total_sum);

    return 0;
}