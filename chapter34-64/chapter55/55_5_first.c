#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/file.h>

int main(int argc, char *arg[]) {

  int fd, i;
  struct flock fl;

  fd = open("aaa", O_RDONLY);
  if (fd == -1)
    errExit("open");

  fl.l_len = 1;
  fl.l_whence = SEEK_SET;
  fl.l_type = F_WRLCK;

  for (i = 0; i < 40001; i++) {
    fl.l_start = i * 2;

    if (fcntl(fd, F_SETLK, &fl) == -1) {
      if (errno == EAGAIN || errno == EACCES)
        printf("locked");
      else if (errno == EDEADLK)
        printf("dead lock");
      else
        errExit("fcntl lock");
    }
  }

  printf("sleep starting....\n");
  sleep(1000);

  exit(EXIT_SUCCESS);
}
