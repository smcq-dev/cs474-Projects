#include "inode.h"
#include "block.h"
#include "free.h"
#include <stddef.h>
#include "pack.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *ialloc(void) {
    unsigned char block[BLOCK_SIZE];
    bread(1, block);
    int free_bit = find_free(block);

    if (free_bit == -1) {
        return NULL;
    }

    struct inode *in = iget(free_bit);

    if (in == NULL) {
        return NULL;
    }

    set_free(block, free_bit, 1);
    bwrite(1, block);
    
    in->size = 0;
    in->owner_id = 0;
    in->permissions = 0;
    in->flags = 0;
    in->link_count = 0;

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        in->block_ptr[i] = 0;
    }

    in->inode_num = free_bit;
    write_inode(in);
    return in;
}

struct inode *incore_find_free(void) {

    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if (incore[i].ref_count == 0) {
            return &incore[i];
        }
    }
    
    return NULL;
}

struct inode *incore_find(unsigned int inode_num) {

    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if (incore[i].ref_count != 0 && incore[i].inode_num == inode_num) {
            return &incore[i];
        }
    }

    return NULL;
}

void incore_free_all(void) {

    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        incore[i].ref_count = 0;
    }
}

void read_inode(struct inode *in, int inode_num) {

    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;

    unsigned char block[BLOCK_SIZE];

    bread(block_num, block);

    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    unsigned char *inode_data = block + block_offset_bytes;

    in->size = read_u32(inode_data);
    in->owner_id = read_u16(inode_data + 4);
    in->permissions = read_u8(inode_data + 6);
    in->flags = read_u8(inode_data + 7);
    in->link_count = read_u8(inode_data + 8);

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        in->block_ptr[i] = read_u16(inode_data + 9 + i * 2);
    }
}

void write_inode(struct inode *in) {

    int block_num = in->inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;

    unsigned char block[BLOCK_SIZE];

    bread(block_num, block);

    int block_offset = in->inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;
    unsigned char *inode_data = block + block_offset_bytes;

    write_u32(inode_data, in->size);
    write_u16(inode_data + 4, in->owner_id);
    write_u8(inode_data + 6, in->permissions);
    write_u8(inode_data + 7, in->flags);
    write_u8(inode_data + 8, in->link_count);

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        write_u16(inode_data + 9 + i * 2, in->block_ptr[i]);
    }

    bwrite(block_num, block);
}

struct inode *iget(int inode_num) {

    struct inode *found = incore_find(inode_num);

    if (found != NULL) {
        found->ref_count++;
        return found;
    }

    struct inode *in = incore_find_free();

    if (in == NULL) {
        return NULL;
    }

    read_inode(in, inode_num);
    in->ref_count = 1;
    in->inode_num = inode_num;
    return in;
}

void iput(struct inode *in) {

    if (in->ref_count == 0) {
        return;
    }

    in->ref_count--;
    if (in->ref_count == 0) {
        write_inode(in);
    }
}