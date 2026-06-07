#include <fcntl.h>
#include <unistd.h>
#include "image.h"
#include "block.h"
#include "free.h"

unsigned char *bread(int block_num, unsigned char *block) {
    int offset = block_num * BLOCK_SIZE;

    lseek(image_fd, offset, SEEK_SET);

    read(image_fd, block, BLOCK_SIZE);
    
    return block;
}

void bwrite(int block_num, unsigned char *block) {
    int offset = block_num * BLOCK_SIZE;

    lseek(image_fd, offset, SEEK_SET);

    write(image_fd, block, BLOCK_SIZE);
}

int alloc(void) {
    unsigned char block[BLOCK_SIZE];
    bread(2, block);
    int free_bit = find_free(block);

    if (free_bit == -1) {
        return -1;
    }

    set_free(block, free_bit, 1);
    bwrite(2, block);
    return free_bit;
}