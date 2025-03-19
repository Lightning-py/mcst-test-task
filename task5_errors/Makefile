.PHONY: all clean

CC=gcc
COMPILE_OPTIONS=-Wall -Wextra -Werror


all: obj
	$(CC) $(COMPILE_OPTIONS) main.o -o result

obj: main.c
	$(CC) $(COMPILE_OPTIONS) main.c -c 

clean:
	rm result *.o
