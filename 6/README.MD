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
  * `CTEST_RESULTS()`: Prints pass/fail summary
  * `CTEST_EXIT()`: Exits with status reflecting test results


## Notes


* This program is being used to build upon for a number of future projects, meaning functionality might be incomplete, and implementation may have to change slightly depending on future requirements, unless the specification for this program is completly write. 
