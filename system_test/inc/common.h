#ifndef __COMMON_H_
#define __COMMON_H_

/*Routine test functions*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROCESS_NUM 2
#define PAGE_SIZE 4096
// #define MEMORY_SIZE 4000000000
#define MEMORY_SIZE 400000000

#define OP_FAIL 2
#define MEMORY_FAULT 3
#define SUCCESS 1
#define FAIL 0
#define MAX_BUF_SIZE 256

int driver(int N);

/*UART info*/
// int UART_info();

/* assigns the cpu and memory division factor
dynamically allocates memory and calls march test 
on the specified memory block
also runs basic ops */
int task(int cpu, int mem_div);

/* checka n queens for N=0 to 12*/
int queens_check();

extern int N;
extern int count;
extern int M;
extern int solution_array[12];

/*checka proc/sys/kernel/printk file*/
int check_printk();

int task_driver();






/*Health Check Functions*/

// Function to read and parse /proc/cpuinfo to get CPU information
void getCPUInfo();

// Function to read and parse /proc/meminfo to get memory information
void get_memory_info() ;

/* Function to get max number of processes*/
int max_processes();

// Function to count the number of running processes
int count_running_processes(); 

// Function to calculate CPU usage for a specific core
int calculate_cpu_usage(int core_id);

#define NUM_CORES 2 // Change this to match the number of CPU cores on the target system


#endif /* __COMMON_H_ */