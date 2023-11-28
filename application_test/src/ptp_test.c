
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdint.h>
#include <string.h>

#include "../inc/common.h"

static int read_line_from_fd(int fd, int size, char *line);

int ptp_check(void)
{
    while (1)
    {
        int fd = open("ptp_log_stream", O_RDONLY);

        if (fd <= 0)
        {
            printf("Opening the ptp log file has failed\n");
            return -1;
        }

        char *value = NULL;
        float offset = 0;

        while (1) 
        {   
            fd_set set;
            FD_ZERO(&set);
            FD_SET(fd, &set);

            /**< Set the timeout as 5 seconds. 
             * Clear the sync bit if the 
             * ptp sync message is not coming for 5 seconds. */
            struct timeval timeout = {
                .tv_sec = 5,
                .tv_usec = 0,
            };

            int retval = select(__FD_SETSIZE, &set, NULL, NULL, &timeout);

            if (retval < 0)
            {
                if (errno == EINTR)
                {
                    /**< Ignore the interrupt error. */
                }
                else
                {
                    /**< Select has failed. Check why*/
                    printf("Select has failed, errno = %d\n", errno);
                    return -2;
                }

                continue;
            }

            if (retval == 0)
            {
                /**< Timeout has happened. */
                printf("Timeout occured\n");
                close(fd);
                return -3;
            }

            char line[1000];

            read_line_from_fd(fd, sizeof(line), line);

            (void) strtok(line, ", ");

            /**< Iterate this fot 5 More Tokens. */
            for (int i = 0; i < 5; i++)
            {
                value = strtok(NULL, ", ");
            }

            if (value == NULL) 
            {
                printf("unable to find ptp offset column\n");
                break;
            }

            errno = 0;

            offset = strtod(value, NULL);

            if (offset == 0 && errno != 0) 
            {
                printf("unable to convert ptp offset float\n");
                break;
            }

            if (offset < 0)
            {
                offset = -offset;
            }

            printf("Offset = %f\n", offset);

            /**< Return from the function. */
            if (offset > 0.001)
            {
                return -1;
            }

            return 0;
        }
        close(fd);
    }  
    return -1;
}

static int read_line_from_fd(int fd, int size, char *line)
{
    for (int i = 0; i < size; i++)
    {
        int status = read(fd, &line[i], sizeof(uint8_t));
        
        if (status <= 0)
        {
            printf("Unable to read\n");
            return -1;
        }

        if (line[i] == '\n')
        {
            line[i] = 0;
            return i;
        }

    }
}