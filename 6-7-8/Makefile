CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: testfs

libvvsfs.a: image.o block.o free.o inode.o mkfs.o pack.o
	ar rcs libvvsfs.a image.o block.o free.o inode.o mkfs.o pack.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

block.o: block.c block.h
	$(CC) $(CFLAGS) -c block.c

free.o: free.c free.h
	$(CC) $(CFLAGS) -c free.c

inode.o: inode.c inode.h
	$(CC) $(CFLAGS) -c inode.c

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -c pack.c

mkfs.o: mkfs.c mkfs.h
	$(CC) $(CFLAGS) -c mkfs.c

testfs.o: testfs.c ctest.h image.h block.h free.h inode.h mkfs.h pack.h
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