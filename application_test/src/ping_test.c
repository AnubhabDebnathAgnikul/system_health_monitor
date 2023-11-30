#include "../inc/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

fc_configuration_t FC_CONFIG;
ec_configuration_t EC_CONFIG;

int ping(const char *ip)
{
    char command[100];
    
    sprintf(command, "ping %s -i 0.1 -w 1 1>/dev/null 2>/dev/null", ip);
    
    int retval = system(command);

    if (retval == 0)
    {
        return 0;
    }
    return -1;
}

int load_config_file(void)
{
    memset(&FC_CONFIG, 0, sizeof(fc_configuration_t));

    char file_path[100];

    /**< stitch the file path. */
    memset(file_path, 0, sizeof(file_path));
    strcat(file_path, folder_path);

    if (this_computer == sys_fc)
    {
        strcat(file_path, "/files/config.bin");
    }
    else
    {
        strcat(file_path, "/files/ec/config.bin");
    }

    /**< Open the configuration file. */
    int file_decriptor = open(file_path, O_RDONLY);

    /**< Unable to open the file. */
    if(file_decriptor == -1)
    {
        return -1;
    }

    /**< Read the configuration into the CONFIG Variable. */
    if (this_computer == sys_fc)
    {
        int bytes_read = read(file_decriptor, 
                                    &FC_CONFIG, sizeof(fc_configuration_t));
        if(bytes_read != sizeof(fc_configuration_t))
        {
            return -1;
        }
    }
    /**< Read the configuration into the CONFIG Variable. */
    if (this_computer == sys_ec)
    {
        int bytes_read = read(file_decriptor, 
                                    &EC_CONFIG, sizeof(ec_configuration_t));
        if(bytes_read != sizeof(ec_configuration_t))
        {
            return -1;
        }
    }
        
    close(file_decriptor);
}