CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: testfs

libvvsfs.a: image.o block.o
	ar rcs libvvsfs.a image.o block.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

block.o: block.c block.h
	$(CC) $(CFLAGS) -c block.c

testfs.o: testfs.c ctest.h image.h block.h
	$(CC) $(CFLAGS) -c testfs.c

testfs: testfs.o libvvsfs.a
	$(CC) $(CFLAGS) -o testfs testfs.o libvvsfs.a

clean:
	rm -f *.o

pristine: 
	rm -f *.o *.a testfs

test: testfs
	./testfs

.PHONY: all clean test pristine