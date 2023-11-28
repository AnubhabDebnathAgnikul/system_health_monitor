#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int ping(const char *ip);
int check_sha256sum(char *reference, char *path);

int main(int argc, char **argv)
{
    /**< Ping */
    char *reference = "02a1979bb05ef0a62e574ae0d1f0982127769167f18ab9e71a8293bba0df9f35";
    char *path = "/home/google/agnikul/code_base/FCU/FC_Software/ReleaseBuild/FC-Release";

    check_sha256sum(reference, path);
}

int ping(const char *ip)
{
    char command[100];
    sprintf(command, "ping %s -i 0.1 -w 1 1>/dev/null 2>/dev/null", ip);
    int retval = system(command);
    return retval;
}

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
    
    int retval = strcmp(buff, reference, 10);

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