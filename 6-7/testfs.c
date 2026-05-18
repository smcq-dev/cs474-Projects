#include "free.h"
#include "inode.h"
#include "image.h"
#include "mkfs.h"
#define CTEST_ENABLE
#include "ctest.h"
#include <stdio.h>
#include "block.h"
#include <string.h>

void image_open_success(void) {
    CTEST_ASSERT(image_open("test.img", 1) != -1, "testing that image open works");
    image_close();
}

void image_open_image_fd_set(void) {
    int fd = image_open("test.img", 1);
    CTEST_ASSERT(fd == image_fd, "testing that calling image open sets image_fd");
    image_close();
}

void image_close_success(void) {
    image_open("test.img", 1);
    CTEST_ASSERT(image_close() != -1, "testing that image close works");
}

void bread_writes_and_reads(void) {
    image_open("image.tst", 1);
    unsigned char buf1[BLOCK_SIZE];
    unsigned char buf2[BLOCK_SIZE];
    unsigned char buf3[BLOCK_SIZE];
    unsigned char buf4[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        buf1[i] = i % 256;
        buf3[i] = i % 256;
    }

    bwrite(0, buf1);
    bread(0, buf2);

    CTEST_ASSERT(memcmp(buf1, buf2, BLOCK_SIZE) == 0, "testing that writing into block 0 and reading from that is accurate");

    bwrite(5, buf3);
    bread(5, buf4);

    CTEST_ASSERT(memcmp(buf3, buf4, BLOCK_SIZE) == 0, "testing that writing into block 5 and reading from that is accurate");
    image_close();
}

void bwrite_doesnt_corrupt_blocks(void) {
    image_open("image.tst", 1);
    unsigned char buf1[BLOCK_SIZE];
    unsigned char buf2[BLOCK_SIZE];
    unsigned char buf3[BLOCK_SIZE];
    unsigned char buf4[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        buf1[i] = i % 256;
    }
    for (int i = 0; i < BLOCK_SIZE; i++) {
        buf2[i] = i % 533;
    }

    bwrite(3, buf1);
    bwrite(4, buf2);
    bread(3, buf3);
    bread(4, buf4);

    CTEST_ASSERT(memcmp(buf1, buf3, BLOCK_SIZE) == 0, "testing that writing into block 3 doesn't corrupt block 4");
    CTEST_ASSERT(memcmp(buf2, buf4, BLOCK_SIZE) == 0, "testing that writing into block 4 doesn't corrupt block 3");
    image_close();
}

void bread_returns_same_pointer(void) {
    image_open("test.img", 1);
    unsigned char buf1[BLOCK_SIZE];
    unsigned char buf2[BLOCK_SIZE];

    bwrite(0, buf1);
    unsigned char *result = bread(0, buf2);

    CTEST_ASSERT(result == buf2, "testing that bread returns pointer to the same buffer passed in");
    image_close();
}

void find_free_returns_zero_block_zero(void) {
    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);
    int result = find_free(block);

    CTEST_ASSERT(result == 0, "testing that a block of all zeros returns 0");
}

void find_free_returns_one_block_one(void) {
    unsigned char block[BLOCK_SIZE];
    memset(block, 0xff, BLOCK_SIZE);
    int result = find_free(block);

    CTEST_ASSERT(result == -1, "testing that a block of all ones returns negative one");
}

void find_free_returns_ten_when_zerotonine_full(void) {
    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);
    for (int i=0; i < 10; i++) {
        set_free(block, i, 1);
    }
    int result = find_free(block);

    CTEST_ASSERT(result == 10, "testing that a block with 0-9 full returns 10");
}

void set_free_when_all_set_returns_that(void) {
    unsigned char block[BLOCK_SIZE];
    memset(block, 0xff, BLOCK_SIZE);
    set_free(block, 309, 0);
    int result = find_free(block);

    CTEST_ASSERT(result == 309, "testing when set free sets block 309 to zero in a block full of 1s, returns 309");
}

void set_free_at_byte_boundary(void) {
    unsigned char block[BLOCK_SIZE];
    memset(block, 0xff, BLOCK_SIZE);
    set_free(block, 8, 0);
    int result = find_free(block);

    CTEST_ASSERT(result == 8, "testing that setting a bit at the byte boundary functions properly");
}

void ialloc_when_all_inodes_free(void) {
    image_open("test.img", 1);
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(1, zeros);  

    int result = ialloc();
    CTEST_ASSERT(result == 0, "first ialloc on empty map returns 0");
}

void ialloc_sequentially_works(void) {
    image_open("test.img", 1);
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(1, zeros);  

    int result1 = ialloc();
    int result2 = ialloc();
    int result3 = ialloc();

    CTEST_ASSERT(result1 == 0, "first ialloc on empty map returns 0");
    CTEST_ASSERT(result2 == 1, "second ialloc on empty map returns 1");
    CTEST_ASSERT(result3 == 2, "third ialloc on empty map returns 2");
}

void alloc_when_all_inodes_free(void) {
    image_open("test.img", 1);
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(2, zeros);  

    int result = alloc();
    CTEST_ASSERT(result == 0, "first alloc on empty map returns 0");
}

void alloc_sequentially_works(void) {
    image_open("test.img", 1);
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(2, zeros);  

    int result1 = alloc();
    int result2 = alloc();
    int result3 = alloc();

    CTEST_ASSERT(result1 == 0, "first alloc on empty map returns 0");
    CTEST_ASSERT(result2 == 1, "second alloc on empty map returns 1");
    CTEST_ASSERT(result3 == 2, "third alloc on empty map returns 2");
}

void mkfs_works(void) {
    mkfs();
    unsigned char block[BLOCK_SIZE];
    bread(2, block);
    CTEST_ASSERT(block[0] == 0x7f, "mkfs marks first 7 blocks");
}



int main(void) {
    image_open_success();
    image_open_image_fd_set();
    image_close_success();
    bread_writes_and_reads();
    bwrite_doesnt_corrupt_blocks();
    bread_returns_same_pointer();
    find_free_returns_one_block_one();
    find_free_returns_zero_block_zero();
    find_free_returns_ten_when_zerotonine_full();
    set_free_when_all_set_returns_that();
    set_free_at_byte_boundary();
    ialloc_when_all_inodes_free();
    ialloc_sequentially_works();
    alloc_when_all_inodes_free();
    alloc_sequentially_works();
    mkfs_works();
    
    CTEST_RESULTS();
    CTEST_EXIT();
}