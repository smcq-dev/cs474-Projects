#include "image.h"
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

int main(void) {
    image_open_success();
    image_open_image_fd_set();
    image_close_success();
    bread_writes_and_reads();
    bwrite_doesnt_corrupt_blocks();
    bread_returns_same_pointer();
    
    CTEST_RESULTS();
    CTEST_EXIT();
}