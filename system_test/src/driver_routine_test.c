#include<stdio.h>
#include<stdlib.h>
#include <syslog.h>
#include "../inc/common.h"

#define TESTING 1

int main()
{
    struct timespec start, end;
    openlog("Initial Checkup", LOG_NDELAY, LOG_USER);
    
    #if TESTING
    clock_gettime(CLOCK_MONOTONIC, &start);
    #endif 

    task_driver();

    if(queens_check()==1)
    {
        // printf("Executed N_Queens Algorithm Successfully\n");
        syslog(LOG_INFO,"Executed N_Queens Algorithm Successfully");
    }
    else
    {
        // printf("N_Queens Algorithm Failed\n");
        syslog(LOG_INFO,"N_Queens Algorithm Failed");
        return FAIL;
        // exit(FAIL);
    }

    if(check_printk()==1)
    {
        // printf("printk is: 0 0 0 0\n");
        syslog(LOG_INFO,"printk is: 0 0 0 0");
    }
    else
    {
        // printf("printk is not zero\n");
        syslog(LOG_CRIT, "printk is not zero");
    }

    closelog();

    #if TESTING
    clock_gettime(CLOCK_MONOTONIC, &end);
    double diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)*pow(10,-9);
    printf("time taken %f seconds\n", diff);
    #endif

    return SUCCESS;
}