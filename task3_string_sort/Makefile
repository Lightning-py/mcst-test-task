.PHONY: all clean

CC=gcc
COMPILE_OPTIONS=-Wall -Wextra -Werror

all: main.c
	$(CC) main.c $(COMPILE_OPTIONS) -o result

clean:
	rm result *.txt

test: all
	./result sorts.c bubble.txt bubblesort
	./result sorts.c merge.txt mergesort
	./result sorts.c quick.txt quicksort
	./result 123 123 123