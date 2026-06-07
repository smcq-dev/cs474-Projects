#include <fcntl.h>
#include <unistd.h>

int image_fd;

int image_open(char *filename, int truncate) {
    if (truncate == 1) {
        image_fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0600);
    }
    if (truncate == 0) {
        image_fd = open(filename, O_RDWR|O_CREAT, 0600);
    }
    return image_fd;
}

int image_close(void) {
    return close(image_fd);
}
