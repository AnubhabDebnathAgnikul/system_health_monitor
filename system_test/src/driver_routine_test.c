#include<stdio.h>
#include<stdlib.h>
#include <syslog.h>
#include "../inc/common.h"

#define TESTING 1

/**
 * @brief Main function for executing various tests and checks.
 *
 * This function performs the following tasks:
 * - Initializes syslog for logging.
 * - Measures the execution time if TESTING is enabled.
 * - Calls task_driver() to execute memory tests and basic ops.
 * - Calls queens_check() to check the N_Queens algorithm.
 * - Calls UART_info() to check if serial lines are commented.
 * - Calls check_printk() to check the printk configuration.
 * - Closes syslog and prints the execution time if TESTING is enabled.
 *
 * @return Returns SUCCESS if all tests pass, FAIL otherwise.
 */


int main()
{
    struct timespec start, end;
    openlog("Initial Checkup", LOG_NDELAY, LOG_USER);
    
    #if TESTING
    clock_gettime(CLOCK_MONOTONIC, &start);
    #endif 

    if(task_driver() == EXIT_SUCCESS)
    {
        syslog(LOG_INFO, "Executed memory test and basic ops");
    }
    else
    {
        syslog(LOG_INFO, "memory and basic ops failed");
        return FAIL;
    }

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
    }

    if(UART_info() == SUCCESS)
    {
        syslog(LOG_INFO, "all lines containing 'tty' are commented");
    }
    else
    {
        syslog(LOG_INFO, "uncommented lines containing 'tty' are found");
        return FAIL;
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