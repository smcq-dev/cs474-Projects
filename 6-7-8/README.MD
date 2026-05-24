# Project 6: Starting the File System


## Building


Command line:

* `make` to build. An executable called `testfs` will be produced.
* `make clean` to clean up all .o build products
* `make pristine` to clean up all build products entirely.
* `make test` to build testfs if necessary and runs it.


## Files


* `.gitignore` to ignore all build products from the github repository
* `block.c` to define all block related logic, for now reading and writing in the blocks.
* `block.h` header file for `block.c`
* `ctest.h` to provide testing logic to use in `testfs.c`
* `image.c` to provide logic for image files, specifically opening and closing them
* `image.h` header file for `image.c`
* `Makefile` to help build the program, clean up executables and create and run the test file
* `testfs.c` for all testing logic of opening and closing images, and reading and writing blocks. 
* `free.c` to provide functions that allow us to find free bits and set bits in a block to in use or free
* `free.h` header for `free.c`
* `inode.c` provides a function ialloc(), that looks at the inode block and finds the first avaiable inode and sets it
* `inode.h`header for `inode.c`
* `mkfs.c` provides a function mkfs() that initializes the file system to have all the blocks zeroed, and sets the first 7 blocks in the block map.
* `mkfs.h` header for `mkfs.c`
* `pack.c` provides functions for packing and unpacking bytes
* `pack.h` header for `pack.c`


## Data


This program uses file descriptors and unix OS syscalls for most of its data. Besides this, we do store data in unsigned char buffers that we use for bwrite and bread. 


## Functions


* `main()`
  * `image_open_success()`: Tests that image_open returns a valid file descriptor
    * `image_open()`: Opens (or creates) the image file
    * `image_close()`: Closes the image file
  * `image_open_image_fd_set()`: Tests that image_open sets the global image_fd
    * `image_open()`
    * `image_close()`
  * `image_close_success()`: Tests that image_close returns successfully
    * `image_open()`
    * `image_close()`
  * `bread_writes_and_reads()`: Tests that data written to a block reads back accurately
    * `image_open()`
    * `bwrite()`: Writes a 4096-byte block to the image file
    * `bread()`: Reads a 4096-byte block from the image file
    * `image_close()`
  * `bwrite_doesnt_corrupt_blocks()`: Tests that writing to one block doesn't affect adjacent blocks
    * `image_open()`
    * `bwrite()`
    * `bread()`
    * `image_close()`
  * `bread_returns_same_pointer()`: Tests that bread returns the pointer it was passed
    * `image_open()`
    * `bwrite()`
    * `bread()`
    * `image_close()`
  * `find_free()`: finds the first free bit in a block
  * `set_free()`: sets a bit in a block to free / in use
  * `ialloc()`: finds the first available inode that isn't in use, and sets in use, writes that to disk and returns the     inode
  * `alloc()`: finds the first available bit in the block map and sets it to in use
  * `mkfs()`: sets inode block and inode data blocks to all zeros and intializes file system
  * `incore_find_free()`: Finds the first free incore inode in the incore inode array.
  * `incore_find()`: Finds an incore inode with a specific inode number in the incore inode array.
  * `incore_free_all()`: Frees all incore inodes in the incore inode array.
  * `read_inode()`: Reads an inode from the inode blocks into a inode struct.
  * `write_inode()`: Writes an inode struct into the proper inode block.
  * `iget()`: Finds an inode and increments its ref_count, then returns it.
  * `iput()`: Returns instantly if the inode ref_count is 0, decrements it if not, if new ref_count is 0 it writes it to disk.
  * `CTEST_RESULTS()`: Prints pass/fail summary
  * `CTEST_EXIT()`: Exits with status reflecting test results


## Notes


* This program is being used to build upon for a number of future projects, meaning functionality might be incomplete, and implementation may have to change slightly depending on future requirements, unless the specification for this program is completly write. 
