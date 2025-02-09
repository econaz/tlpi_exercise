

#include "tlpi_hdr.h"
#include <error_functions.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int dup3(int oldfd);
int dup4(int oldfd, int newfd);

int main(int argc, char *argv[]) {
  int newfd;

  newfd = dup4(1, 1);
  if (newfd == -1)
    errExit("exit");
  printf("the newfd is %d", newfd);
}

int dup3(int oldfd) { return fcntl(oldfd, F_DUPFD, oldfd); }
int dup4(int oldfd, int newfd) {

  int flags, startfd;
  flags = fcntl(oldfd, F_GETFL);
  if (flags == -1)
    errExit("not exit oldfd");
  if (oldfd == newfd)
    return newfd;
  startfd = fcntl(oldfd, F_DUPFD, newfd);
  if (startfd == -1)
    errExit("copy fd error");
  return startfd;
}
