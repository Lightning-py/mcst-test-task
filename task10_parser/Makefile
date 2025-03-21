.PHONY: all clean obj

CC=g++
COMPILE_OPTIONS=-Wall -Wextra # -Werror


all: obj
	$(CC) $(COMPILE_OPTIONS) main.o -o result

obj: main.cpp
	$(CC) $(COMPILE_OPTIONS) main.cpp -c 

clean:
	rm result *.o 