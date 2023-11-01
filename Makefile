#This Makefile is only used for test
VERSION = 0.1.0
TESTS := $(wildcard tests/*.sh)

TEST_DIR = ./tests
CC = riscv64-linux-gnu-gcc

test:
	${CC} -B${TEST_DIR} -static ${TEST_DIR}/main.o -o ${TEST_DIR}/a.out



