#!/bin/bash

# Run the C program and capture its output
result=$(./main)

# Check the exit status of the C program
if [ $? -eq 1 ]; then
    echo "System Validation Success"
    ./health_check
else
    echo "Failure, abort!"
fi
