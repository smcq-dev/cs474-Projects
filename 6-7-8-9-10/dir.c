#include "inode.h"
#include "pack.h"
#include "dir.h"
#include "block.h"
#include "dirbasename.h"
#include "pathname.h"
#include <stdlib.h>
#include <string.h>

struct directory *directory_open(int inode_num) {
    struct inode *in = iget(inode_num);
    if (in == NULL) {
        return NULL;
    }

    struct directory *dir = malloc(sizeof(struct directory));

    if (dir == NULL) {
        return NULL;
    }

    dir->inode = in;
    dir->offset = 0;
    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent) {

    if (dir->inode->size <= dir->offset) {
        return -1;
    }

    int data_block_index = dir->offset / BLOCK_SIZE;

    int data_block_num = dir->inode->block_ptr[data_block_index];

    unsigned char block[BLOCK_SIZE];

    bread(data_block_num, block);

    int offset_in_block = dir->offset % BLOCK_SIZE;

    ent->inode_num = read_u16(block + offset_in_block);
    strcpy(ent->name, (char *)(block + offset_in_block + 2));

    dir->offset += DIRECTORY_ENTRY_SIZE;

    return 0;
}

void directory_close(struct directory *d) {
    iput(d->inode);
    free(d);
}

int directory_make(char *path) {
    char dirname[1024];
    char basename[1024];

    get_dirname(path, dirname);
    get_basename(path, basename);

    struct inode *p = namei(dirname);

    if (p == NULL) {
        return -1;
    }

    struct inode *newd = ialloc();

    if (newd == NULL) {
        return -1;
    }

    int data_block_num = alloc();

    if (data_block_num == -1) {
        return -1;
    }

    newd->flags = INODE_FLAG_DIRECTORY;
    newd->size = DIRECTORY_ENTRY_SIZE * 2;
    newd->block_ptr[0] = data_block_num;

    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);
    write_u16(block + 0, newd->inode_num);
    strcpy((char *)(block + 2), ".");

    write_u16(block + 32, p->inode_num);
    strcpy((char *)(block + 34), "..");
    bwrite(data_block_num, block);

    int data_block_index = p->size / BLOCK_SIZE;
    int parent_data_block_num = p->block_ptr[data_block_index];
    int offset_in_block = p->size % BLOCK_SIZE;

    unsigned char parent_block[BLOCK_SIZE];
    bread(parent_data_block_num, parent_block);

    write_u16(parent_block + offset_in_block, newd->inode_num);
    strcpy((char *)(parent_block + offset_in_block + 2), basename);

    bwrite(parent_data_block_num, parent_block);

    p->size += DIRECTORY_ENTRY_SIZE;

    iput(newd);
    iput(p);

    return 0;
}
