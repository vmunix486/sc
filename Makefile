CC=gcc
CFLAGS=-pedantic -Wall -Wextra -ansi -Ofast -flto
#CPPFLAGS=-D_DEBUG
RM=rm
RMFLAGS=-fv

all: main

main:
	$(CC) $(CFLAGS) $(CPPFLAGS) src/main.c -o sci

clean:
	$(RM) $(RMFLAGS) sci
