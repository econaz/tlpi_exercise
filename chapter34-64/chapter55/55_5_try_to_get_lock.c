#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/file.h>

int main(int argc, char *arg[]) {

  int fd, i, n;
  struct flock fl;

  n = getInt(arg[1], GN_ANY_BASE, "number");

  fd = open("aaa", O_RDONLY);
  if (fd == -1)
    errExit("open");

  fl.l_whence = SEEK_SET;
  fl.l_type = F_WRLCK;
  fl.l_len = 1;

  for (i = 0; i < 10000; i++) {
    fl.l_start = i * 2;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
      if (errno != EAGAIN && errno != EWOULDBLOCK) {
        if (errno == EDEADLK)
          printf("dead lock");
        else
          errExit("setlk");
      }
    } else {
      printf("locked\n");
    }
  }
  exit(EXIT_SUCCESS);
}
