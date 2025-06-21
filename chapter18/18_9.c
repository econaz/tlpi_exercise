

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int fchdir_c() {

  int fd_old, fd_new, i = 0;
  char *curpath;

  fd_new = open("~/Downloads", O_RDONLY);
  fd_old = open(".", O_RDONLY);
  for (i = 0; i < 100; i++) {
    fchdir(fd_new);
    fchdir(fd_old);
  }
  return -1;
}

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }
