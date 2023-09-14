#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

#include <time.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROCESS_NUM 2
#define PAGE_SIZE 4096
#define MEMORY_SIZE 4000000000
// #define MEMORY_SIZE 40000
void*address_list;

#define OP_FAIL 2
#define MEMORY_FAULT 3
#define SUCCESS 4

/* Random number generator taking current time as the seed*/
long int random()
{
    time_t t;
    time(&t);
    srand(t);

    int n = (rand()%360)+1;
    return n;
}

/* Swap operaion without a third vaiable to chek
addition, negation, assignment and comparison*/
int swap()
{
    int a = random();
    int b = random();

    int real_a = a;
    int real_b = b;

    a = a+b;
    b = a-b;
    a = a-b;

    if(b==real_a && a==real_b)
    return 1;
    else
    return 0; 
}


/* trigonometric functions & identities*/
int trig()
{
    double epsilon = 0.0001;
    int theta = random(); 

    double a = sin(theta);
    double b = cos(theta);
    double c = tan(theta);

    double result = a*a+ b*b;
    // printf("div= %f tan=%f\n",(a/b),c);

    if(((a/b)-c) < epsilon)
        return 1;
    else
        return 0;
}

/* Logical operation check*/
int logical()
{
    time_t t;
    int bits = 8;
    unsigned char my_char =  240;
    unsigned char f = 255;
    unsigned char my_char_not = ~my_char;
    srand(time(&t));
    unsigned char n = rand();

    //and & not
    unsigned char lsb = my_char_not & n;
    unsigned char msb = my_char & n;

    //xor & or
    unsigned char msb_l = n^lsb;
    unsigned char lsb_l = n^msb;    
    // printf("%d %d", n,lsb_l|msb_l);

    //shift
    unsigned char n_shift = (n<<4)| (n>>(bits-4)) ;
    // printf("%d %d %d %d", lsb,msb,lsb_l,msb_l);
    // printf("%d %d %d",n,(msb>>4)|(lsb<<4),n_shift);

    if((lsb==lsb_l)&&(msb==msb_l)&&(n==(lsb_l|msb_l))&&((msb>>4)|(lsb<<4) == n_shift))
        return 1;
    else
        return 0;
}


/* function to find the physical address corresponding to a given virtual address pointer*/
unsigned long long physical_address(void* virtualAddress)
{
     // Open the /proc/self/pagemap file to access page mapping information
    int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
    if (pagemap_fd < 0) 
    {
        printf("Failed to open pagemap file.\n");
        return 1;
    }

    // Calculate the virtual address page number
    unsigned long long virtualPageNumber = (unsigned long long)virtualAddress / PAGE_SIZE;

    // Seek to the corresponding entry in the pagemap file
    off_t offset = virtualPageNumber * sizeof(unsigned long long);
    if (lseek(pagemap_fd, offset, SEEK_SET) == -1) 
    {
        printf("Failed to seek to the pagemap entry.\n");
        return 1;
    }

    // Read the entry from the pagemap file
    unsigned long long pagemapEntry;
    if (read(pagemap_fd, &pagemapEntry, sizeof(unsigned long long)) == -1) {
        printf("Failed to read the pagemap entry.\n");
        return 1;
    }
    // printf("page entry: %0llx\n",pagemapEntry);
    // Extract the physical page frame number from the pagemap entry
    unsigned long long physicalPageNumber = pagemapEntry & 0x7FFFFFFFFFFFFF;

    // Calculate the physical address
    unsigned long long physicalAddress = (physicalPageNumber * PAGE_SIZE) +
                                         ((unsigned long long)virtualAddress % PAGE_SIZE);

    printf("Virtual Address: %p\n", virtualAddress);
    printf("Physical Address: %llx\n", physicalAddress);
    return physicalAddress;

    // Cleanup
    close(pagemap_fd);

}

/* March Pattern test on a meomry block pointed by the argument pointer*/
int march(unsigned char*p)
{
    int x =1,flag=0;
    unsigned char a[8] = {1,2,4,8,16,32,64,128}; // hardcoded array for comparison
    for(int k=0;k<8;k++)
    {
       *p = x<<k;   // b'1 is left shifted 
        // printf("value=%02x\n",*p);
        if(*p!=a[k])
        {
            flag=1;
            printf("Fault Dtected \n");
            physical_address(p);
        }       
    }
    return flag;
}

/* assigns the cpu and memory division factor
dynamically allocates memory and calls march test 
on the specified memory block*/
int task(int cpu, int mem_div)
{
    
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu,&set);
    sched_setaffinity(getpid(),sizeof(cpu_set_t),&set);
    printf("executing in cpu: %d ",cpu);

    for(int i=0;i<100;i++)
    {
        // printf("check %d\n",i);
        if(swap()!=1 || trig()!=1 || logical()!=1)
        {
            printf("Basic Ops Failed");
            return OP_FAIL;
        } 
    }
    printf("Basic Ops Success\n");

    void* virtualAddress = malloc(MEMORY_SIZE/mem_div);
    unsigned char* temp_adress = virtualAddress;
    if (virtualAddress == NULL) 
    {
        printf("Failed to allocate memory.\n");
        return 1;
    }
    // physical_address(virtualAddress);
    // sphysical_address(temp_adress);

    for(int i=0;i<MEMORY_SIZE/mem_div;i++)
    {
        // printf("count: %d\n",i);
        int check = march(temp_adress);
        
        if(check==1)
        {
            // printf("fault detected \n",temp_adress);
            return MEMORY_FAULT;
        }   
        temp_adress++;
    }
    free(virtualAddress);
}

int main()
{
    pid_t pid[PROCESS_NUM];
    int main_pid = getpid(),status;
    // printf("parent process: %d\n",main_pid);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i=0; i<PROCESS_NUM;i++)
    {
        if((pid[i]= fork())== 0 )
        {
            //children
            task(i,2);
            printf("executed process %d\n", getpid());
            exit(i);
        }
    }

    for (int i=0;i<PROCESS_NUM;i++)
    {
        pid_t cpid = waitpid(pid[i],&status,0);
        if(WIFEXITED(status))
        {
            // printf("Child %d terminated with exit status %d\n", cpid,WEXITSTATUS(status));
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)*pow(10,-9);
    printf("time taken %f seconds\n", diff);
    return 0;
}