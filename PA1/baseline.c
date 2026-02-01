/**
 * File that calculates the sum (non-inclusive) of variables 0-N.
 * Divides the workload evenly among different
 * number of threads.
 * Uses: Pthread libary
 * Author: Nick Schwartz and Robert Rodarte
 * Date: 1/31/2026
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

long long N = 1000000;

int main(int argc, char *argv[])
{
    // Handle command line inputs
    if (argc >= 2)
    {
        N = atoll(argv[1]);
    }
    long long sum = 0;

    for (long long i = 0; i < N; i++)
    {
        sum = sum + i;
    }
    printf("Sum = %lld\n", sum);

    return 0;
}