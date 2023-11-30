#include "../inc/common.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int check_sha256sum(void)
{
    char command[1000];
    
    /**< Compute cksm from be binary. */
    if (this_computer == sys_ec)
    {
        sprintf(command, "sha256sum %s/%s", folder_path, "EC-Release");
    }
    else
    {
        sprintf(command, "sha256sum %s/%s", folder_path, "FC-Release");
    }

    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        return -1;
    }

    char buff[100];
    fscanf(fp, "%s", buff);
    pclose(fp);

    /**< Read reference from the cksm.txt. */
    char reference_path[100];
    strcpy(reference_path, folder_path);
    strcat(reference_path, "/cksm.txt");

    fp = fopen(reference_path, "r");
    if (fp == NULL)
    {
        return -1;
    }
    char reference[100];
    fscanf(fp, "%s", reference);
    fclose(fp);
    
    int retval = strncmp(buff, reference, 64);

    if (retval == 0)
    {
        retval = 0;
    }
    else
    {
        retval = -1;
    }

    return retval;
}