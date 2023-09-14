#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>

#define MAX_BUF_SIZE 128
#define NUM_CORES 4 // Change this to match the number of CPU cores on the target system


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
void getMemoryInfo() 
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

// Function to count the number of running processes
int countRunningProcesses() 
{
    FILE *fp;
    char buffer[MAX_BUF_SIZE];
    int count = 0;
    
    fp = popen("ps -e | wc -l", "r");
    if (fp == NULL) 
    {
        perror("Error executing ps command");
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), fp)) 
    {
        count = atoi(buffer);
    }

    pclose(fp);
    
    return count;
}

// Function to count the number of threads in the current process
int countThreads() 
{
    int count = 0;
    char buffer[MAX_BUF_SIZE];
    FILE *fp;
    char line[256];

    // fp = fopen("/proc/self/status", "r");  // threads in current process
    fp = popen("ps -eo nlwp | tail -n +2 | awk '{ num_threads += $1 } END { print num_threads }'","r");
    if (fp == NULL) 
    {
        perror("Error executing ps command");
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), fp)) 
    {
        count = atoi(buffer);
    }

    pclose(fp);
    return count;
}

 



// Function to calculate CPU usage for a specific core
void* calculate_cpu_usage(void* arg) {
    int core_id = *((int*)arg);
    FILE* file = NULL;
    char filename[128];
    unsigned long long total_time[2], idle_time[2];

    snprintf(filename, sizeof(filename), "/proc/stat");

    while (1) 
    {
        file = fopen(filename, "r");
        if (file == NULL) 
        {
            perror("fopen");
            exit(1);
        }

        char line[256];
        char* cpu_line = NULL;
        while (fgets(line, sizeof(line), file)) 
        {
            if (line[0] == 'c' && line[1] == 'p' && line[2] == 'u' && line[3] == core_id + '0') 
            {
                cpu_line = line;
                break;
            }
        }
        fclose(file);

        if (cpu_line) 
        {
            unsigned long long user, nice, system, idle;
            sscanf(cpu_line, "cpu%d %llu %llu %llu %llu", &core_id, &user, &nice, &system, &idle);

            total_time[1] = user + nice + system + idle;
            idle_time[1] = idle;

            if (total_time[0] > 0) 
            {
                unsigned long long total_delta = total_time[1] - total_time[0];
                unsigned long long idle_delta = idle_time[1] - idle_time[0];
                double usage = 100.0 * (1.0 - ((double)idle_delta / (double)total_delta));

                printf("Core %d CPU Usage: %.2f%%\n", core_id, usage);
            }

            total_time[0] = total_time[1];
            idle_time[0] = idle_time[1];
        }
        sleep(1); // Sleep for 1 second before checking again
        
        // system("clear"); 
    }
}


void *monitor() 
{
    while(1)
    {
        // printf("CPU Health Monitor\n");

        // Get CPU information
        // getCPUInfo();

        // Get memory information
        getMemoryInfo();

        // Get the number of running processes
        int runningProcesses = countRunningProcesses();
        if (runningProcesses >= 0) 
        {
            printf("Running Processes: %d\n", runningProcesses);
        }

        usleep(1000000);
        // system("clear");
    }

    return 0;
}

int main() 
{
    pthread_t threads[NUM_CORES+1];
    int core_ids[NUM_CORES];
    
    for (int i = 0; i < NUM_CORES; i++) 
    {
        core_ids[i] = i;
        pthread_create(&threads[i], NULL, calculate_cpu_usage, &core_ids[i]);
    }
    pthread_create(&threads[NUM_CORES], NULL, monitor, NULL);

    for (int i = 0; i < NUM_CORES; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
    
}

