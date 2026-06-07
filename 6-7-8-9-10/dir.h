#ifndef DIR_H
#define DIR_H
#define DIRECTORY_ENTRY_SIZE 32
#define INODE_FLAG_DIRECTORY 2

struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[16];
};

struct directory *directory_open(int inode_num);
int directory_get(struct directory *dir, struct directory_entry *ent);
void directory_close(struct directory *d);
int directory_make(char *path);



#endif