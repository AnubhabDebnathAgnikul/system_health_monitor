#!/bin/bash

designation="EC"

if [ "$designation" = "FC" ];
then
    path='/tmp/fc_test/'
fi

if [ "$designation" = "EC" ]; 
then
    path='/tmp/ec_test/'
fi

# logger -t "ST" "Starting System Test...."
# result=$(/root/system_test)
# logger -t "ST" "Starting System Test Complete"

# # Check the exit status of the C program
# if [ $? -eq 1 ]; then
#     logger -t "ST" "System Validation Success"
#     #./health_check  
# else
#     logger -t "ST" "Failure, abort!"
#     exit 1
# fi

# Start PTP.
if [ "$designation" = "FC" ]; then
    ptpd2 -i switch -s -U -S $path/files/ptp_log_stream 1>/dev/null 2>/dev/null
fi

if [ "$designation" = "EC" ]; then
    ptpd2 -i eth0 -s -U -S $path/files/ec/ptp_log_stream 1>/dev/null 2>/dev/null
fi

logger -t "ST" "Starting Application Test...."
# Run the application test and capture its output
result=$(./app_test $path $designation )
logger -t "ST" "Starting Application Complete"

# Check the exit status of the C program
if [ $? -eq 0 ]; then
    logger -t "ST" "Application Validation Success"
    #./health_check  
else
    logger -t "ST" "Failure, abort!"
    exit 
fi