# object = basic_op.c
# object = integrated.c
# object = routine_test.c
object=main.c

# CC = gcc
# CC =/home/anubhab/Desktop/Agnikul/cross_compiler/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
CC=$(NXP_COMPILER_PATH)
all:
	@$(CC) $(object) -o main -lm
	
clean:
	rm -rf main 

rebuild: clean all