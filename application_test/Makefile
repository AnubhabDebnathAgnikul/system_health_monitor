object=main.c

# CC = gcc
# CC =/home/anubhab/Desktop/Agnikul/cross_compiler/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
CC=$(NXP_COMPILER_PATH)

C_FILES  = src/checksum_test.c
C_FILES += src/ping_test.c
C_FILES += src/ptp_test.c

all: $(C_FILES)
	@$(CC) $(object) -o app_test -lm $(C_FILES)
	
send:
	sshpass -p 'root' scp main root@1.1.1.22:~/

clean:
	rm -rf app_test 

rebuild: clean all