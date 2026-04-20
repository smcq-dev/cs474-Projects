CC=gcc
CCOPTS=-Wall -Wextra

.PHONY: all clean

all: myalloc

myalloc: myalloc.c
	$(CC) $(CCOPTS) -o myalloc myalloc.c

clean:
	rm -f myalloc