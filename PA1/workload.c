#include <stdio.h>
#include <stdlib.h>


__int128_t main(int start, int stop){
    __int128_t sum =0;
    for (int i = start; i < stop;i++) {
        sum = sum + i;
    }
    printf("Sum = %ld\n", sum);
    
    return 0;}


