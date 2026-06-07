#include "pathname.h"
#include "inode.h"
#include "dirbasename.h"
#include "dir.h"
#include <stddef.h>
#include <string.h>

struct inode *namei(char *path) {

    if (strcmp(path, "/") == 0) {
        struct inode *in = iget(ROOT_INODE_NUM);
        return in;
    }

    char basename[1024];
    get_basename(path, basename);

    struct directory *dir = directory_open(ROOT_INODE_NUM);
    if (dir == NULL) {
        return NULL;
    }

    struct directory_entry ent;
    int found_inode = -1;

    while(directory_get(dir, &ent) != -1) {
        if (strcmp(ent.name, basename) == 0) {
            found_inode = ent.inode_num;
            break;
        }
    }

    directory_close(dir);

    if (found_inode == -1) {
        return NULL;
    }

    return iget(found_inode);
}
