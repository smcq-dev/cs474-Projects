CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: testfs

libvvsfs.a: image.o block.o free.o inode.o mkfs.o pack.o dir.o ls.o pathname.o dirbasename.o
	ar rcs libvvsfs.a image.o block.o free.o inode.o mkfs.o pack.o dir.o ls.o pathname.o dirbasename.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

pathname.o: pathname.c pathname.h
	$(CC) $(CFLAGS) -c pathname.c

dirbasename.o: dirbasename.c dirbasename.h
	$(CC) $(CFLAGS) -c dirbasename.c

ls.o: ls.c ls.h
	$(CC) $(CFLAGS) -c ls.c

dir.o: dir.c dir.h
	$(CC) $(CFLAGS) -c dir.c

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

testfs.o: testfs.c ctest.h image.h block.h free.h inode.h mkfs.h pack.h dir.h ls.h pathname.h dirbasename.h
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