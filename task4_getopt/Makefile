.PHONY: all clean

CC=gcc
COMPILE_OPTIONS=-Wall -Wextra -Werror


all: obj
	$(CC) $(COMPILE_OPTIONS) main.o options.o -o result

obj: main.c options.c options.h
	$(CC) $(COMPILE_OPTIONS) main.c options.c -c 

clean:
	rm result *.o
