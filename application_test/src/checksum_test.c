#include "../inc/common.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int check_sha256sum(char *reference, char *path)
{
    char command[100];
    sprintf(command, "sha256sum %s", path); 

    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        printf("popen errno = %d\n", errno);
        return 0;
    }

    char buff[100];
    fscanf(fp, "%s", buff);
    
    int retval = strncmp(buff, reference, 64);

    if (retval == 0)
    {
        retval = 0;
    }
    else
    {
        retval = -1;
    }

    pclose(fp);
    return retval;
}