#!/bin/bash

designation="FC"

if [ "$designation" = "FC" ];
then
    path='/root/fc_test/'
fi

if [ "$designation" = "EC" ]; 
then
    path='/root/ec_test/'
fi

result=$(./health_check)

# Check the exit status of the C program
# if [ $? -eq 1 ]; then
#     echo "System Validation Success"
#     #./health_check  
# else
#     echo "Failure, abort!"
#     exit 1
# fi

# # Start PTP.
# if [ "$designation" = "FC" ]; then
#     ptpd2 -i switch -s -U -S $path/files/ptp_log_stream 1>/dev/null 2>/dev/null
# fi

# if [ "$designation" = "EC" ]; then
#     ptpd2 -i switch -s -U -S $path/files/ec/ptp_log_stream 1>/dev/null 2>/dev/null
# fi

# # Run the application test and capture its output
# result=$(./app_test /root/fc_test/)

# # Check the exit status of the C program
# if [ $? -eq 1 ]; then
#     echo "Application Validation Success"
#     #./health_check  
# else
#     echo "Failure, abort!"
#     exit 
# fi