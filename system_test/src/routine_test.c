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


void *address_list;


/**
 * @brief Generate a random long integer.
 *
 * This function generates a random long integer based on the current time.
 *
 * @return Returns a random long integer.
 */
long int random()
{
    // Get the current time
    time_t t;
    time(&t);

    // Seed the random number generator with the current time
    srand(t);

    // Generate a random number between 1 and 360
    int n = (rand() % 360) + 1;

    // Return the generated random number
    return n;
}


/**
 * @brief Swap the values of two integers.
 *
 * This function generates two random integers, swaps their values without using a temporary variable,
 * and checks if the swap operation is successful.
 *
 * @return Returns 1 if the swap is successful, 0 otherwise.
 */
int swap()
{
    // Generate two random integers
    int a = random();
    int b = random();

    // Preserve the original values for later comparison
    int real_a = a;
    int real_b = b;

    // Perform the swap without using a temporary variable
    a = a + b;
    b = a - b;
    a = a - b;

    // Check if the swap is successful by comparing with the original values
    if (b == real_a && a == real_b)
        return 1; // Return 1 to indicate a successful swap
    else
        return 0; // Return 0 to indicate a failed swap
}


/**
 * @brief Trigonometric function and identities test.
 *
 * This function generates a random angle and calculates sine, cosine, tangent,
 * and a trigonometric identity. It then checks the identity and returns the result.
 *
 * @return Returns 1 if the trigonometric identity is within a specified epsilon value,
 *         otherwise returns 0.
 */
int trig()
{
    // Define a small value for epsilon to compare floating-point numbers
    double epsilon = 0.0001;

    // Generate a random angle (theta) for trigonometric calculations
    int theta = random(); 

    // Calculate sine, cosine, and tangent of the random angle
    double a = sin(theta);
    double b = cos(theta);
    double c = tan(theta);

    // Calculate a trigonometric identity using sine and cosine
    double result = a * a + b * b;

    // Uncomment the next line to print additional information
    // printf("div= %f tan=%f\n",(a/b),c);

    // Check if the calculated identity satisfies a condition within epsilon
    if (((a / b) - c) < epsilon)
        return 1;
    else
        return 0;
}


/**
 * @brief Performs various bitwise operations and logical checks.
 *
 * This function generates a random unsigned char, performs bitwise operations,
 * and checks the logical consistency of the results.
 *
 * @return Returns 1 if the logical checks pass, 0 otherwise.
 */
int logical()
{
    // Seed for random number generation
    time_t t;
    srand(time(&t));

    // Initialize variables for bitwise operations
    int bits = 8;
    unsigned char my_char = 240;
    unsigned char f = 255;
    unsigned char my_char_not = ~my_char;
    unsigned char n = rand();

    // Perform AND and NOT operations
    unsigned char lsb = my_char_not & n;
    unsigned char msb = my_char & n;

    // Perform XOR and OR operations
    unsigned char msb_l = n ^ lsb;
    unsigned char lsb_l = n ^ msb;

    // Perform left shift and right shift operations
    unsigned char n_shift = (n << 4) | (n >> (bits - 4));

    // Check logical consistency
    if ((lsb == lsb_l) && (msb == msb_l) && (n == (lsb_l | msb_l)) && ((msb >> 4) | (lsb << 4) == n_shift))
        return 1; // Logical checks passed
    else
        return 0; // Logical checks failed
}





int N;                  ///< Size of the chessboard
int count = 0;   ///< Counter for the number of solutions
int M = 12;          ///< Maximum board size for checking
int solution_array[] = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200}; ///< Precomputed solutions


/**
 * @brief Checks if it's safe to place a queen at the specified position on the board.
 *
 * @param board The chessboard represented as a 2D array.
 * @param row Row index.
 * @param col Column index.
 * @return Returns true if it's safe to place a queen, false otherwise.
 */
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

/**
 * @brief Solves the N-Queens problem using backtracking.
 *
 * @param board The chessboard represented as a 2D array.
 * @param col Current column index.
 * @return Returns true if a solution is found, false otherwise.
 */
bool solveNQueens(int board[N][N], int col) {
    // Base case: If all queens are placed
    if (col >= N) {
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

/**
 * @brief Driver function to run the N-Queens algorithm.
 *
 * @param localN The board size for the N-Queens problem.
 * @return Returns the number of solutions found.
 */
int driver(int localN) {
    N = localN;
    int board[N][N]; // Initialize the chessboard
    memset(board, 0, N * N * sizeof(int));
    if (!solveNQueens(board, 0)) {
        // No solution exists.
    }
    return count;
}

/**
 * @brief Checks N Queens for board sizes from 1 to M.
 *
 * @return Returns SUCCESS if the precomputed solutions match, FAIL otherwise.
 */
int queens_check() {
    int green_flag = 0;
    for (int i = 1; i <= M; i++) {
        count = 0;
        N = i;
        if (solution_array[i - 1] != driver(N)) {
            green_flag = 0;
            return FAIL;
        }
        green_flag = 1;
    }
    return green_flag;
}




/**
 * @brief Retrieves information about serial ports from the /etc/inittab file.
 *
 * This function reads the /etc/inittab file line by line, checks for lines containing "tty",
 * and determines if the line is uncommented. It returns a flag indicating the presence of
 * uncommented lines containing "tty".
 *
 * @return Returns SUCCESS if no uncommented lines containing "tty" are found, FAIL otherwise.
 */
int UART_info() {
    FILE *fp;
    char filename[] = "/etc/inittab";
    char buffer[MAX_BUF_SIZE];
    int flag = SUCCESS; // Default value assuming no uncommented lines containing "tty" are found

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        syslog(LOG_CRIT,"failed to open /etc/inittab");
        return(FAIL);
    }

    // Read the file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Check for lines containing "tty"
        if (strstr(buffer, "tty")) {
            // Check if the line is uncommented
            if (buffer[0] != '#') {
                flag = FAIL; // Uncommented line containing "tty" found
            }
        }
    }

    // Close the file
    fclose(fp);

    // Return the flag indicating the presence of uncommented lines containing "tty"
    return flag;
}



/**
 * @brief Finds the physical address corresponding to a given virtual address pointer.
 *
 * This function opens the /proc/self/pagemap file to access page mapping information.
 * It calculates the virtual address page number, seeks to the corresponding entry in
 * the pagemap file, and extracts the physical page frame number from the pagemap entry.
 * The physical address is then calculated using the page frame number and the offset within the page.
 * The virtual and physical addresses are logged to syslog.
 *
 * @param virtualAddress Pointer to the virtual address for which the physical address is sought.
 * @return Returns the physical address if successful, or 1 if an error occurs.
 */
unsigned long long physical_address(void* virtualAddress)
{
    // Open the /proc/self/pagemap file to access page mapping information
    int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
    if (pagemap_fd < 0) 
    {
        syslog(LOG_CRIT, "Failed to open pagemap file");
        return FAIL;
    }

    // Calculate the virtual address page number
    unsigned long long virtualPageNumber = (unsigned long long)virtualAddress / PAGE_SIZE;

    // Seek to the corresponding entry in the pagemap file
    off_t offset = virtualPageNumber * sizeof(unsigned long long);
    if (lseek(pagemap_fd, offset, SEEK_SET) == -1) 
    {
        syslog(LOG_CRIT, "Failed to seek to the pagemap entry");
        return FAIL;
    }

    // Read the entry from the pagemap file
    unsigned long long pagemapEntry;
    if (read(pagemap_fd, &pagemapEntry, sizeof(unsigned long long)) == -1) {
        syslog(LOG_CRIT, "Failed to read the pagemap entry");
        return FAIL;
    }

    // Extract the physical page frame number from the pagemap entry
    unsigned long long physicalPageNumber = pagemapEntry & 0x7FFFFFFFFFFFFF;

    // Calculate the physical address
    unsigned long long physicalAddress = (physicalPageNumber * PAGE_SIZE) +
                                         ((unsigned long long)virtualAddress % PAGE_SIZE);

    // Log virtual and physical addresses to syslog
    syslog(LOG_CRIT, "Virtual Address: %p", virtualAddress);
    syslog(LOG_CRIT, "Physical Address: %llx", physicalAddress);

    // Cleanup
    close(pagemap_fd);

    return physicalAddress;
}



/**
 * @brief Runs the March Pattern test on a memory block pointed by the argument pointer.
 *
 * This function applies the March Pattern test to the memory block pointed by the argument pointer.
 * The function uses a hardcoded array for comparison and checks if the memory values match the expected pattern.
 * If a mismatch is detected, it logs a fault message to syslog and prints the physical address of the fault.
 *
 * @param p Pointer to the memory block to be tested.
 * @return Returns 0 if the test passes, 1 if a fault is detected.
 */
int march(unsigned char* p)
{
    // syslog(LOG_INFO, "Running March Pattern test in memory");
    int x = 1, flag = 0;
    unsigned char a[8] = {1, 2, 4, 8, 16, 32, 64, 128}; // Hardcoded array for comparison

    for (int k = 0; k < 8; k++)
    {
        *p = x << k; // b'1 is left-shifted
        // printf("value=%02x\n", *p);

        if (*p != a[k])
        {
            flag = 1;
            // printf("Fault Detected \n");
            syslog(LOG_CRIT, "MARCH PATTERN TEST: FAULT DETECTED");
            physical_address(p);
        }
    }

    return flag;
}



/**
 * @brief Assigns the CPU and memory division factor, allocates memory, and performs tests.
 *
 * This function sets the CPU affinity, executes basic operations in a loop,
 * allocates memory, and runs the March test on the specified memory block.
 * It also logs information about CPU execution and test results to syslog.
 *
 * @param cpu CPU core to assign affinity.
 * @param mem_div Memory division factor for allocating memory.
 * @return Returns OP_FAIL if basic operations fail, MEMORY_FAULT if a memory fault is detected, 0 otherwise.
 */
int task(int cpu, int mem_div)
{
    // syslog(LOG_INFO, "Running March Pattern test in memory");
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    sched_setaffinity(getpid(), sizeof(cpu_set_t), &set);
    // printf("executing in cpu: %d ",cpu);
    syslog(LOG_INFO, "Executing in CPU: %d", cpu);

    for (int i = 0; i < 100; i++)
    {
        // printf("check %d\n", i);
        if (swap() != 1 || trig() != 1 || logical() != 1)
        {
            // printf("Basic Ops Failed");
            syslog(LOG_CRIT, "Basic Ops Failed");
            return FAIL;
            // exit(FAIL);
        }
    }
    // printf("Basic Ops Success\n");
    syslog(LOG_INFO, "Basic Ops Success");

    void *virtualAddress = malloc(MEMORY_SIZE / mem_div);
    unsigned char *temp_address = virtualAddress;

    if (virtualAddress == NULL)
    {
        // printf("Failed to allocate memory.\n");
        syslog(LOG_CRIT, "Failed to allocate memory");
        return FAIL;
        // exit(FAIL);
    }
    // physical_address(virtualAddress);
    // sphysical_address(temp_address);

    for (int i = 0; i < MEMORY_SIZE / mem_div; i++)
    {
        // printf("count: %d\n", i);
        int check = march(temp_address);

        if (check == 1)
        {
            // printf("Fault detected \n", temp_address);
            return MEMORY_FAULT;
            exit(FAIL);
        }
        temp_address++;
    }
    free(virtualAddress);
    return SUCCESS;
}

/**
 * @brief Driver code to spawn multiple processes, execute tasks, and handle their termination.
 *
 * This function creates multiple child processes using fork, each executing the task function.
 * It waits for the termination of each child process, logs the exit status, and checks for successful termination.
 *
 * @return Returns EXIT_SUCCESS if all child processes terminate successfully, EXIT_FAILURE otherwise.
 */
int task_driver()
{
    pid_t pid[PROCESS_NUM];
    int main_pid = getpid(),status;

    for (int i=0; i<PROCESS_NUM; i++)
    {
        if((pid[i]=fork())==0)
        {
            // children
            if(task(i,PROCESS_NUM)!=FAIL)
            {
                syslog(LOG_INFO,"Executed Memory test and Basic Ops");
                exit(EXIT_SUCCESS);
            }
            exit(EXIT_FAILURE);
        }
    }

    int exit_status[PROCESS_NUM] = {-1};

    for (int i=0; i<PROCESS_NUM; i++)
    {
        pid_t cpid = wait(&exit_status[i]);

        if (WIFEXITED(exit_status[i]))
        {
            syslog(LOG_INFO, "exited, status = %d\n",WEXITSTATUS(exit_status[i]));
        }
        else if (WIFSIGNALED(exit_status[i]))
        {
            syslog(LOG_INFO, "Killed by signal %d\n",WTERMSIG(exit_status[i]));
        }
        else if (WIFSTOPPED(exit_status[i]))
        {
            syslog(LOG_INFO,"stopped by signal %d\n",WSTOPSIG(exit_status[i]));
        }
        else if (WIFCONTINUED(exit_status[i]))
        {
            syslog(LOG_INFO, "continued");
        }
    }

    for (int i = 0; i<PROCESS_NUM; i++)
    {
        if(exit_status[i]!=0)
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Checks the current printk configuration in the kernel.
 *
 * This function opens the "/proc/sys/kernel/printk" file, reads its content line by line,
 * and checks if the printk configuration is set to "0 0 0 0". It returns 1 if the configuration
 * matches, indicating that printk is configured with zeros, and returns 0 otherwise.
 *
 * @return Returns 1 if printk is configured with "0 0 0 0", 0 otherwise.
 */
int check_printk()
{
    FILE *fp;
    char buffer[MAX_BUF_SIZE];

    // Open the /proc/sys/kernel/printk file for reading
    fp = fopen("/proc/sys/kernel/printk", "r");

    if (fp == NULL)
    {
        syslog(LOG_CRIT,"Error opening /proc/sys/kernel/printk");
        return FAIL;
    }

    // Read the content of the file line by line
    while (fgets(buffer, sizeof(buffer), fp))
    {
        // Check if the printk configuration is "0 0 0 0"
        if (buffer[0] == '0' && buffer[2] == '0' && buffer[4] == '0' && buffer[6] == '0')
        {
            fclose(fp);
            return SUCCESS; // printk is configured with "0 0 0 0"
        }
        else
        {
            fclose(fp);
            return FAIL; // printk configuration does not match
        }
    }
}
