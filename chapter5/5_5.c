

#include "tlpi_hdr.h"
#include <error_functions.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void checkfd(int oldfd, int newfd);

int main(int argc, char *argv[]) {
  int oldfd, newfd;
  off_t old_off;
  oldfd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

  if (oldfd == -1)
    errExit("open");

  old_off = lseek(oldfd, 4, SEEK_SET);
  if (oldfd == -2)
    errExit("lseek");
  newfd = dup(oldfd);
  if (newfd == -1)
    errExit("dup");

  checkfd(oldfd, newfd);
}

void checkfd(int oldfd, int newfd) {
  int oldflags, newflags, oldaccess, newaccess;
  off_t oldoff, newoff;
  oldflags = fcntl(oldfd, F_GETFL);
  if (oldflags == -1)
    errExit("oldfd fcntl error");
  oldaccess = oldflags & O_ACCMODE;
  newflags = fcntl(newfd, F_GETFL);
  if (newflags == -1)
    errExit("newfd fcntl error");
  newaccess = newflags & O_ACCMODE;
  if (oldaccess == newaccess)
    printf("the access mode is true\n");
  else
    printf("the access mode is not true\n");

  oldoff = lseek(oldfd, 0, SEEK_CUR);
  if (oldoff == -1)
    errExit("old lseek error");
  newoff = lseek(newfd, 0, SEEK_CUR);
  if (newoff == -1)
    errExit("new lseek error");

  if (oldoff == newoff)
    printf("the off_t is same");
  else
    printf("the off_t is not same");
}
