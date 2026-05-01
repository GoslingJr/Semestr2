CC=gcc
CFLAGS=-Wall -Wextra -std=c11

all: main test

main:
	$(CC) main.c dfa.c -o main

test:
	$(CC) test.c dfa.c -o test

run-tests: test
	./test
