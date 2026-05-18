#include "free.h"
#include "block.h"

int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;
    
    return -1;
}

int find_free(unsigned char *block) {
    
    for (int i = 0; i < BLOCK_SIZE; i++) {
       
        int res = find_low_clear_bit(block[i]);

        if(res != -1) {
            return res + i * 8;
        }
    }
    return -1;
}

void set_free(unsigned char *block, int num, int set) {
    int byte_num = num / 8;  
    int bit_num = num % 8;

    if (set == 1) {
        block[byte_num] |= (1 << bit_num);
    }

    else if (set == 0) {
        block[byte_num] &= ~(1 << bit_num);
    }
}