CC=gcc
CFLAGS=-pedantic -Wall -Wextra -ansi -Ofast -flto
#CPPFLAGS+=-D_DEBUG
#CPPFLAGS+=-D_NO_MEMSET
RM=rm
RMFLAGS=-fv

all: main

main:
	$(CC) $(CFLAGS) $(CPPFLAGS) src/main.c -o sci

clean:
	$(RM) $(RMFLAGS) sci
