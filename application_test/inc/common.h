/**
 ******************************************************************************
 * @file			common.h
 * @project         contains function definitions.
 * @brief			
 * @author			Sai Thilak
 * @version			v1.0
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) Agnikul Cosmos Private Limited
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

/*******************************************************************************
 * Header inclusions
 ******************************************************************************/

#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdint.h>
#include <syslog.h>
enum
{
    sys_ec,
    sys_fc,
};

typedef struct __attribute__((packed))
{
    int telem_enable;
    char FC_IP[30];
    char EC_IP[30];
    char LCC_IP[30];
    char TELE_IP[30];
    char ATS_IP[30];
    int FC_PORT;
    int EC_PORT;
    int LCC_PORT;
    int ATS_PORT;
	uint32_t daq_spi_speed;
	uint32_t daq_loop_time;
	uint16_t erx_priority;
	int erx_timeout;
	uint8_t flight_start_phase;
	float temperature_lt;
	float temperature_ut;
	float voltage_lt;
	float voltage_ut;
	float current_lt;
	float current_ut;
	float ptp_offset_max;
	int max_cpu_usage;
}ec_configuration_t;

typedef struct __attribute__((packed))
{
    int telem_enable;
    char FC_IP[30];
    char EC_IP[30];
    char LCC_IP[30];
    char TELE_IP[30];
    char ATS_IP[30];
    int FC_PORT;
    int EC_PORT;
    int LCC_PORT;
    int ATS_PORT;
    int erx_timeout;
    int lmp_cycles;
    int print_cdt;
    float ptp_offset_max;
    uint8_t flight_start_phase;
}fc_configuration_t;

/**
 * @brief This function is used to check if a host is alive.
 *  This sends icmp packets at an interval of 100ms for 1 second.
 * @param ip - IP address of the HOST.
 * @return int - Returns 0 if HOST is available.
 * @return int - Returns -1 if HOST is not available.
 */
int ping(const char *ip);

/**
 * @brief This function is used to check whether ptp has started properly.
 * 
 * @return int 
 */
int ptp_check(void);

/**
 * @brief 
 * 
 * @return int 
 */
int check_sha256sum(void);

int load_config_file(void);

extern char folder_path[200];
extern int this_computer;
extern fc_configuration_t FC_CONFIG;
extern ec_configuration_t EC_CONFIG;

#endif /* __COMMON_H_ */