

#define _FILE_OFFSET_BITS 64
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

  off_t off;
  int fd;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname offset\n", argv[0]);

  fd = open(argv[1], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1)
    errExit("open");

  off = atoll(argv[2]);
  if (lseek(fd, off, SEEK_SET) == -1)
    errExit("lseek");

  if (write(fd, "test", 4) == -1)
    errExit("write");

  exit(EXIT_SUCCESS);
}
