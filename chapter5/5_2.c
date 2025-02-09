

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFSIZE 1024
int main(int argc, char *argv[]) {
  int fd;
  off_t offset;
  char buf[BUFFSIZE];

  fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1)
    errExit("open");

  offset = lseek(fd, 0, SEEK_SET);
  printf("%ld", offset);
  if (offset == -1)
    errExit("lseek");
  if (write(fd, "test", 4) != 4)
    errExit("write");
  exit(EXIT_SUCCESS);
}
