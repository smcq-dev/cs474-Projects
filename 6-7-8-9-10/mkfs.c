#include "mkfs.h"
#include "block.h"
#include "inode.h"
#include "pack.h"
#include <string.h>
#include "stdio.h"


void mkfs(void) {
    unsigned char zeros[BLOCK_SIZE];
    memset(zeros, 0, BLOCK_SIZE);
    bwrite(0, zeros);  
    bwrite(1, zeros);
    bwrite(2, zeros);
    bwrite(3, zeros);
    bwrite(4, zeros);
    bwrite(5, zeros);
    bwrite(6, zeros);

    for (int i=0; i < 7; i++) {
        alloc();
    }

    struct inode *dir = ialloc();
    int free_bit = alloc();
    dir->flags = 2;
    dir->size = 64;
    dir->block_ptr[0] = free_bit;

    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);
    write_u16(block, dir->inode_num);
    strcpy((char *)(block + 2), ".");
    
    write_u16(block + 32, dir->inode_num);
    strcpy((char *)(block + 34), "..");
    bwrite(free_bit, block);

    iput(dir);
}