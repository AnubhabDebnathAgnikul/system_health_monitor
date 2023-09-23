#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>
#include <dirent.h>

#define MAX_BUF_SIZE 128
#define NUM_CORES 4 // Change this to match the number of CPU cores on the target system
unsigned long long total_time[NUM_CORES][2]={0}, idle_time[NUM_CORES][2]={0};

// Function to read and parse /proc/cpuinfo to get CPU information
void getCPUInfo() 
{
    FILE *fp;
    char buffer[MAX_BUF_SIZE];
    
    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) 
    {
        perror("Error opening /proc/cpuinfo");
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp)) 
    {
        if (strstr(buffer, "model name")) 
        {
            printf("CPU Model: %s", strchr(buffer, ':') + 2);
            break;
        }
    }

    fclose(fp);
}


// Function to read and parse /proc/meminfo to get memory information
void get_memory_info() 
{
    FILE *fp;
    char buffer[MAX_BUF_SIZE];
    
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) 
    {
        perror("Error opening /proc/meminfo");
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp)) 
    {
        if (strstr(buffer, "MemTotal")) 
        {
            printf("Total Memory: %s", strchr(buffer, ':') + 2);
        } 
        else if (strstr(buffer, "MemFree")) 
        {
            printf("Free Memory: %s", strchr(buffer, ':') + 2);
        }
    }

    fclose(fp);
}

/* Function to get max number of processes*/
int max_processes()
{
    FILE *fp;
    char filename[] = "/proc/sys/kernel/pid_max";
    int pid_max;

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open /proc/sys/kernel/pid_max");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fp, "%d", &pid_max) != 1) {
        perror("Failed to read value from /proc/sys/kernel/pid_max");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    printf("Max allowed processes: %d\n",pid_max);
    // Close the file
    fclose(fp);

    return pid_max;
}

// Function to count the number of running processes
int count_running_processes() 
{
    DIR *dir;
    struct dirent *entry;
    int process_count = 0;

    
    // Open the /proc directory
    dir = opendir("/proc");

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Iterate through the directory entries
    while ((entry = readdir(dir))) {
        // Check if the entry is a directory and its name is numeric (a process directory)
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            process_count++;
        }
    }
    max_processes();
    printf("Number of running processes %d\n", process_count);
    // Close the directory
    closedir(dir);  
    return process_count;
}


// Function to calculate CPU usage for a specific core
int calculate_cpu_usage(int core_id)
{       
    FILE* fp = fopen("/proc/stat","r");
    if (fp == NULL) 
    {
        perror("Failed to open");
        exit(1);
    }
    char line[256];
    char* cpu_line = NULL;
    while(fgets(line,sizeof(line),fp))
    {
        if (line[0] == 'c' && line[1] == 'p' && line[2] == 'u' && line[3] == core_id + '0') 
            {
                cpu_line = line;
                break;
            }
    }
    fclose(fp);
    if (cpu_line) 
        {
            unsigned long long user = 0, nice = 0, system = 0, idle = 0, numbers[7],sum=0;
            char* ptr = NULL;
            char* token= strtok_r(cpu_line," ",&ptr);
            for (int i = 0; i < 4; i++) 
            {
                token = strtok_r(NULL, " ",&ptr);
                if (token == NULL) 
                {
                    printf("Not enough tokens in the line\n");
                    break;
                }
                switch (i) {
                    case 0:
                        user = strtoull(token, NULL, 10);
                        break;
                    case 1:
                        nice = strtoull(token, NULL, 10);
                        break;
                    case 2:
                        system = strtoull(token, NULL, 10);
                        break;
                    case 3:
                        idle = strtoull(token, NULL, 10);
                        break;
                }
            }   
                        
            total_time[core_id][1] = user + nice + system + idle;

            idle_time[core_id][1] = idle;

            if (total_time[core_id][0] >= 0) 
            {
                unsigned long long total_delta = total_time[core_id][1] - total_time[core_id][0];
                
                unsigned long long idle_delta = idle_time[core_id][1] - idle_time[core_id][0];
                double usage = 100.0 * (1.0 - ((double)idle_delta / (double)total_delta));

                printf("Core %d CPU Usage: %.2f%%\n", core_id, usage);
            }

            total_time[core_id][0] = total_time[core_id][1];
            idle_time[core_id][0] = idle_time[core_id][1];
        }
    return 0;
}

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
