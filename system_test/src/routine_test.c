#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <syslog.h>

#include <time.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../inc/common.h"


/* Driver Function to run N-Queens Algorithm */
/* N queens ALGORITHM */
int N ;     // Change N to the desired board size
int count = 0;
int M = 12;
int solution_array[] = {1,0,0,2,10,4,40,92,352,724,2680,14200};
void *address_list;

// #define PROCESS_NUM 2
// #define PAGE_SIZE 4096
// // #define MEMORY_SIZE 4000000000
// #define MEMORY_SIZE 400000000
// void*address_list;

// #define OP_FAIL 2
// #define MEMORY_FAULT 3
// #define SUCCESS 1
// #define FAIL 0
// #define MAX_BUF_SIZE 256

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

// /* N queens ALGORITHM */
// int N ;     // Change N to the desired board size
// static int count = 0;
// int M = 12;
// int solution_array[] = {1,0,0,2,10,4,40,92,352,724,2680,14200};
// Function to check if it's safe to place a queen at board[row][col]
bool isSafe(int board[N][N], int row, int col) {
    // Check row on the left side
    for (int i = 0; i < col; i++) {
        if (board[row][i]) {
            return false;
        }
    }

    // Check upper diagonal on the left side
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    // Check lower diagonal on the left side
    for (int i = row, j = col; i < N && j >= 0; i++, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    return true;
}

// Function to solve the N-Queens problem
bool solveNQueens(int board[N][N], int col) {
    // Base case: If all queens are placed
    if (col >= N) {
        // printBoard(board);
        count++;
        return true;
    }

    bool result = false;

    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col)) {
            board[i][col] = 1;

            // Recur to place the rest of the queens
            result = solveNQueens(board, col + 1) || result;

            // If placing a queen in board[i][col] doesn't lead to a solution,
            // then remove the queen from board[i][col]
            board[i][col] = 0;
        }
    }

    return result;
}

/* Driver Function to run N-Queens Algorithm */
int driver(int localN) 
{
    N = localN;
    int board[N][N]; // Initialize the chessboard
    memset(board, 0 , N*N*sizeof(int));
    if (!solveNQueens(board, 0)) {
        // printf("No solution exists.\n");
    }
    // printf("%d\n",count);
    return count;
}

/* checka n queens for N=0 to 12*/
int queens_check()
{
    int green_flag = 0;
    for(int i = 1 ; i<=M; i++)
    {
        count = 0;
        N=i;        
        if(solution_array[i-1]!=driver(N))
        {
            // printf("NOO!\n");
            // printf("%d ----- %d\n",solution_array[i-1],driver(N));
            green_flag = 0;
            return FAIL;
            // exit(FAIL);
        }
        green_flag = 1;
    }
    return green_flag;
}

/*UART info*/
// int UART_info()
// {
//     FILE *fp;
//     char filename[] = "/etc/inittab";
//     char buffer[MAX_BUF_SIZE];
//     int flag = 1;

//     //open file for reading
//     fp = fopen(filename,"r");
//     if(fp==NULL)
//     {
//         perror("failed to open /etc/inittab");
//         // exit(FAIL);
//     }

//     while(fgets(buffer,sizeof(buffer),fp))
//     {
//         if(strstr(buffer,"tty"))
//         {
//             if(buffer[0]!='#')
//             {
//                 flag=0;
//             }
//         }
//     }
//     return flag;
// }



/* function to find the physical address corresponding to a given virtual address pointer*/
unsigned long long physical_address(void* virtualAddress)
{
     // Open the /proc/self/pagemap file to access page mapping information
    int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
    if (pagemap_fd < 0) 
    {
        // printf("Failed to open pagemap file.\n");
        syslog(LOG_CRIT,"Failed to open pagemap file");
        return 1;
    }

    // Calculate the virtual address page number
    unsigned long long virtualPageNumber = (unsigned long long)virtualAddress / PAGE_SIZE;

    // Seek to the corresponding entry in the pagemap file
    off_t offset = virtualPageNumber * sizeof(unsigned long long);
    if (lseek(pagemap_fd, offset, SEEK_SET) == -1) 
    {
        // printf("Failed to seek to the pagemap entry.\n");
        syslog(LOG_CRIT,"Failed to seek to the pagemap entry");
        return 1;
    }

    // Read the entry from the pagemap file
    unsigned long long pagemapEntry;
    if (read(pagemap_fd, &pagemapEntry, sizeof(unsigned long long)) == -1) {
        // printf("Failed to read the pagemap entry.\n");
        syslog(LOG_CRIT,"Failed to read the pagemap entry");
        return 1;
    }
    // printf("page entry: %0llx\n",pagemapEntry);
    // Extract the physical page frame number from the pagemap entry
    unsigned long long physicalPageNumber = pagemapEntry & 0x7FFFFFFFFFFFFF;

    // Calculate the physical address
    unsigned long long physicalAddress = (physicalPageNumber * PAGE_SIZE) +
                                         ((unsigned long long)virtualAddress % PAGE_SIZE);

    // printf("Virtual Address: %p\n", virtualAddress);
    // printf("Physical Address: %llx\n", physicalAddress);
    syslog(LOG_CRIT,"Virtual Address: %p\n", virtualAddress);
    syslog(LOG_CRIT,"Physical Address: %llx\n", physicalAddress);
    return physicalAddress;

    // Cleanup
    close(pagemap_fd);

}

/* March Pattern test on a meomry block pointed by the argument pointer*/
int march(unsigned char*p)
{
    // syslog(LOG_INFO,"running march pattern test in memory");
    int x =1,flag=0;
    unsigned char a[8] = {1,2,4,8,16,32,64,128}; // hardcoded array for comparison
    for(int k=0;k<8;k++)
    {
       *p = x<<k;   // b'1 is left shifted 
        // printf("value=%02x\n",*p);
        if(*p!=a[k])
        {
            flag=1;
            // printf("Fault Dtected \n");
            syslog(LOG_CRIT,"MARCH PATTERN TEST: FAULT DETECTED");
            physical_address(p);
        }       
    }
    return flag;
}

/* assigns the cpu and memory division factor
dynamically allocates memory and calls march test 
on the specified memory block
also runs basic ops */
int task(int cpu, int mem_div)
{
    // syslog(LOG_INFO,"running march pattern test in memory");
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu,&set);
    sched_setaffinity(getpid(),sizeof(cpu_set_t),&set);
    // printf("executing in cpu: %d ",cpu);
    syslog(LOG_INFO,"executing in cpu: %d ",cpu);

    for(int i=0;i<100;i++)
    {
        // printf("check %d\n",i);
        if(swap()!=1 || trig()!=1 || logical()!=1)
        {
            // printf("Basic Ops Failed");
            syslog(LOG_CRIT,"Basic Ops Failed");
            return OP_FAIL;
            exit(FAIL);
        } 
    }
    // printf("Basic Ops Success\n");
    syslog(LOG_INFO, "Basic Ops Success");

    // if(UART_info()==1)
    // {
    //     printf("serial (UART) disabled");
    // }
    // else
    // {
    //     printf("serial (UART) enabled");
    // }

    void* virtualAddress = malloc(MEMORY_SIZE/mem_div);
    unsigned char* temp_adress = virtualAddress;
    if (virtualAddress == NULL) 
    {
        // printf("Failed to allocate memory.\n");
        syslog(LOG_CRIT,"Failed to allocate memory");
        return 1;
        exit(FAIL);
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
            exit(FAIL);
        }   
        temp_adress++;
    }
    free(virtualAddress);
}


int task_driver()
{
    pid_t pid[PROCESS_NUM];
    int main_pid = getpid(),status;

    for (int i=0; i<PROCESS_NUM;i++)
    {
        if((pid[i]= fork())== 0 )
        {
            //children
            task(i,PROCESS_NUM);
            // printf("executed process %d\n", getpid());
            syslog(LOG_INFO, "Executed memory and basic Ops");
            // return 0;
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
}


/*checka proc/sys/kernel/printk file*/
int check_printk()
{
    FILE *fp;
    char buffer[MAX_BUF_SIZE];

    fp = fopen("/proc/sys/kernel/printk", "r");

    if(fp==NULL)
    {
        perror("error opening /proc/sys/kernel/printk");
        return 0;
    }

    while(fgets(buffer, sizeof(buffer),fp))
    {
        if(buffer[0]=='0' && buffer[2]=='0' && buffer[4]=='0' && buffer[6]=='0')
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}