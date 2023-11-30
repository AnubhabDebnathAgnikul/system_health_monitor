#include "inc/common.h"
#include <stdio.h>
#include <string.h>

char folder_path[200];
int this_computer = sys_ec;

int main(int argc, char **argv)
{
    /**< STAP stands for Startup Application Test. */
    openlog("[STAP]", LOG_NDELAY, LOG_USER);

    if (argv[1] == NULL)
    {
        syslog(LOG_INFO, "Invalid Argument\n");
        return -1;
    }

    if (argv[2] == NULL)
    {
        syslog(LOG_INFO, "Invalid Argument\n");
        return -1;
    }

    strcpy(folder_path, argv[1]);

    if (strcmp(argv[2],"EC") == 0)
    {
        this_computer = sys_ec;
    }
    else if (strcmp(argv[2],"FC") == 0)
    {
        this_computer = sys_fc;
    }
    else
    {
        syslog(LOG_INFO, "Invalid Argument\n");
        return -1;
    }

    /**< Perform Checksum test. */
    if (check_sha256sum() != 0)
    {
        syslog(LOG_INFO, "Checksum failure\n");
        return -1;
    }

    load_config_file();

    if (this_computer == sys_fc)
    {
        /**< Ping to ATS, LCC and MCRC. */
        if (ping(FC_CONFIG.ATS_IP) != 0)
        {
            syslog(LOG_INFO, "ping test to ATS has failed\n");
            return -1;
        }
        if (ping(FC_CONFIG.LCC_IP) != 0)
        {
            syslog(LOG_INFO, "ping test to LCC has failed\n");
            return -1;
        }
        /**< TODO: Add MCRC IP Address. */
        // if (ping(FC_CONFIG.MCRC_IP) != 0)
        // {
        //     syslog(LOG_INFO, "ping test to MCRC has failed\n");
        //     return -1;
        // }
    }

    if (this_computer == sys_ec)
    {
        /**< Ping to ATS, LCC and MCRC. */
        if (ping(EC_CONFIG.ATS_IP) != 0)
        {
            syslog(LOG_INFO, "ping test to ATS has failed\n");
            return -1;
        }
        // if (ping(EC_CONFIG.FC_IP) != 0)
        // {
        //     syslog(LOG_INFO, "ping test to FC has failed\n");
        //     return -1;
        // }
        // if (ping(EC_CONFIG.LCC_IP) != 0)
        // {
        //        syslog(LOG_INFO, "ping test to LCC has failed\n");
        //     return -1;
        // }
        /**< TODO: Add MCRC IP Address. */
        // if (ping(EC_CONFIG.MCRC_IP) != 0)
        // {
        //     syslog(LOG_INFO, "ping test to MCRC has failed\n");
        //     return -1;
        // }
    }

    if (ptp_check() != 0)
    {
        syslog(LOG_INFO, "PTP test has failed\n");
        return -1;
    }
    return 0;
}