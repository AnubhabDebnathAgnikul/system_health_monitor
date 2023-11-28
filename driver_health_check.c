#include<stdio.h>
#include "functions.h"
// #define NUM_CORES 2 // Change this to match the number of CPU cores on the target system

/* driver */
int main() 
{
    
    while(1)
   {
        get_memory_info();
        count_running_processes();
        for(int i=0; i<NUM_CORES;i++)
        {
            calculate_cpu_usage(i);
        }
        printf("\n\n");
        usleep(1000000);
   }

    return 0;
    
}