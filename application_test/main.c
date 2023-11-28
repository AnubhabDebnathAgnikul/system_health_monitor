#include "inc/common.h"
#include <stdio.h>
#include <string.h>

char folder_path[200];

int main(int argc, char **argv)
{
    strcpy(folder_path, argv[1]);

    char *reference = "d74f497e31694f7e31e14bce1fac57d9231afb726f293f500ae4b79db1507b1b";
    char *path = "FC-Release";

    int retval = check_sha256sum(reference, path);
    printf("checksum retval = %d\n", retval);

    printf("retval = %d\n", ping("1.1.1.1"));
    printf("retval = %d\n", ping("1.1.2.1"));
    printf("retval = %d\n", ping("1.1.2.2"));
    printf("PTP check = %d\n", ptp_check());

}