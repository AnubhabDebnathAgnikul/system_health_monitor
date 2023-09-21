# object = basic_op.c
# object = integrated.c
object = routine_test.c

CC = gcc
# CC =/home/anubhab/Desktop/Agnikul/cross_compiler/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

all:
	@$(CC) $(object) -o main -lm
	
clean:
	@if [ -f main ]; then\
		rm main;\
		echo "previous main file removed";\
	fi
rebuild:
	@if [ -f main ]; then\
		rm main;\
		echo "previous main file removed";\
	fi
	@$(CC) $(object) -o main -lm;\
	echo "main file rebuilt";\