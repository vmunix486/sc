CC=gcc
CFLAGS=-pedantic -Wall -Wextra -ansi -Ofast -flto
RM=rm
RMFLAGS=-fv

all: main

main:
	$(CC) $(CFLAGS) src/main.c -o sci

clean:
	$(RM) $(RMFLAGS) sci
