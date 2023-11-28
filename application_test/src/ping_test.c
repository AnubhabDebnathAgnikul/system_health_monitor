#include "../inc/common.h"
#include <stdio.h>
#include <stdlib.h>

int ping(const char *ip)
{
    char command[100];
    sprintf(command, "ping %s -i 0.1 -w 1 1>/dev/null 2>/dev/null", ip);
    int retval = system(command);
    return retval;
}