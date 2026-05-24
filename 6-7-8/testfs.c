#include "free.h"
#include "inode.h"
#include "image.h"
#include "mkfs.h"
#define CTEST_ENABLE
#include "ctest.h"
#include <stdio.h>
#include "block.h"
#include <string.h>
#include "pack.h"

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
    mkfs();
    incore_free_all();

    struct inode *result = ialloc();
    CTEST_ASSERT(result != NULL, "ialloc returns non-NULL on first call");
    CTEST_ASSERT(result->inode_num == 0, "first ialloc returns inode 0");
    image_close();
}

void ialloc_sequentially_works(void) {
    image_open("test.img", 1);
    mkfs();
    incore_free_all();

    struct inode *result1 = ialloc();
    struct inode *result2 = ialloc();
    struct inode *result3 = ialloc();

    CTEST_ASSERT(result1->inode_num == 0, "first ialloc returns inode 0");
    CTEST_ASSERT(result2->inode_num == 1, "second ialloc returns inode 1");
    CTEST_ASSERT(result3->inode_num == 2, "third ialloc returns inode 2");
    image_close();
}

void alloc_when_all_inodes_free(void) {
    image_open("test.img", 1);
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(2, zeros);  

    int result = alloc();
    CTEST_ASSERT(result == 0, "first alloc on empty map returns 0");
    image_close();
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
    image_close();
}

void mkfs_works(void) {
    image_open("test.img", 1);
    mkfs();
    unsigned char block[BLOCK_SIZE];
    bread(2, block);
    CTEST_ASSERT(block[0] == 0x7f, "mkfs marks first 7 blocks");
    image_close();
}

void incore_find_free_works(void) {
    incore_free_all();

    struct inode *in = incore_find_free();
    
    CTEST_ASSERT(in != NULL, "incore_find_free returns an inode");
}

void incore_find_free_returns_null(void) {
    incore_free_all();

    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        struct inode *in = incore_find_free();
        in->ref_count = 1;
    }

    struct inode *in = incore_find_free();

    CTEST_ASSERT(in == NULL, "incore_find_free returns NULL when full");
}

void incore_find_returns_modified_slot(void) {
    incore_free_all();

    struct inode *in = incore_find_free();
    in->ref_count = 1;
    in->inode_num = 20;

    struct inode *found = incore_find(20);

    CTEST_ASSERT(found == in, "incore_find returns a modified inode");
}

void incore_find_returns_null(void) {
    incore_free_all();

    struct inode *found = incore_find(99);

    CTEST_ASSERT(found == NULL, "incore_find returns NULL when inode doesn't exist");
}

void incore_find_returns_null_inode_free(void) {
    incore_free_all();

    struct inode *in = incore_find_free();
    in->ref_count = 0;
    in->inode_num = 99;

    struct inode *found = incore_find(99);

    CTEST_ASSERT(found == NULL, "when an inode number is free, incore_find will return NULL when looking for it");
}

void write_and_read_inodes(void) {
    image_open("test.img", 1);

    mkfs();

    struct inode in;
    in.size = 1000;
    in.owner_id = 20;
    in.permissions = 7;
    in.flags = 0;
    in.link_count = 3;
    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        in.block_ptr[i] = i * 10;
    }
    in.inode_num = 5;

    write_inode(&in);

    struct inode in2;
    in2.inode_num = 5;

    read_inode(&in2, 5);

    CTEST_ASSERT(in.size == in2.size, "Inode sizes match");
    CTEST_ASSERT(in.owner_id == in2.owner_id, "Inode oids match");
    CTEST_ASSERT(in.permissions == in2.permissions, "Inode perms match");
    CTEST_ASSERT(in.flags == in2.flags, "Inode flags match");
    CTEST_ASSERT(in.link_count == in2.link_count, "Inode link_counts match");

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        CTEST_ASSERT(in.block_ptr[i] == in2.block_ptr[i], "Inode block ptr matches");
    }
    image_close();
}

void iget_returns_same_ptr_multiple_calls(void) {
    image_open("test.img", 1);
    mkfs();
    incore_free_all();
    struct inode *p1 = iget(5);
    struct inode *p2 = iget(5);

    CTEST_ASSERT(p1 == p2, "Both pointers are the same");
    CTEST_ASSERT(p1->ref_count == 2, "Ref count incremented twice");
    image_close();
}

void iget_returns_null(void) {
    incore_free_all();

    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        struct inode *plh = incore_find_free();
        plh->ref_count = 1;
        plh->inode_num = i;
    }

    struct inode *p = iget(999);

    CTEST_ASSERT(p == NULL, "iget returns NULL when there isn't an inode with the number");
}

void iput_decrements_ref_count(void) {
    image_open("test.img", 1);
    mkfs();
    incore_free_all();
    iget(5);
    struct inode *p = iget(5);
    iput(p);

    CTEST_ASSERT(p->ref_count == 1, "iput decrements ref_count each call");
    image_close();
}

void iput_writes_to_disk(void) {
    image_open("test.img", 1);
    mkfs();
    incore_free_all();
    struct inode *p = iget(5);
    p->size = 5555;

    iput(p);
    incore_free_all();
    struct inode *p1 = iget(5);

    CTEST_ASSERT(p1->size == 5555, "iput writes to disk when ref_count drops to 0");
    image_close();
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
    incore_find_free_works();
    incore_find_free_returns_null();
    incore_find_returns_modified_slot();
    incore_find_returns_null();
    incore_find_returns_null_inode_free();
    write_and_read_inodes();
    iget_returns_same_ptr_multiple_calls();
    iget_returns_null();
    iput_decrements_ref_count();
    
    CTEST_RESULTS();
    CTEST_EXIT();
}