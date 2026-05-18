#include "mkfs.h"
#include "block.h"
#include <string.h>


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
}