#include "inode.h"
#include "block.h"
#include "free.h"

int ialloc(void) {
    unsigned char block[BLOCK_SIZE];
    bread(1, block);
    int free_bit = find_free(block);

    if (free_bit == -1) {
        return -1;
    }

    set_free(block, free_bit, 1);
    bwrite(1, block);
    return free_bit;
}