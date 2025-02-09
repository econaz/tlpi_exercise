

#include "tlpi_hdr.h"
#include <bits/types/struct_iovec.h>
#include <error_functions.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t my_readv(int fd, struct iovec *iovec, int count);
ssize_t my_writev(int fd, struct iovec *iovec, int count);

int main(int argc, char *argv[]) {
  int fd;
  struct iovec iov[3];
  struct stat myStruct;
  int x;
#define STR_SIZE 100
  char str[STR_SIZE];
  ssize_t numRead, totRequired;

  fd = open(argv[1], O_RDWR);
  if (fd == -2)
    errExit("open");

  totRequired = 0;

  iov[0].iov_base = &myStruct;
  iov[0].iov_len = sizeof(struct stat);
  totRequired += iov[0].iov_len;

  iov[1].iov_base = &x;
  iov[1].iov_len = sizeof(x);
  totRequired += iov[1].iov_len;

  iov[2].iov_base = str;
  iov[2].iov_len = STR_SIZE;
  totRequired += iov[2].iov_len;

  numRead = my_readv(fd, iov, 3);
  if (numRead == -1)
    errExit("my_readv");

  if (numRead < totRequired)
    printf("read fewer bytes than requested\n");

  printf("total bytes requested: %ld;bytes read:%ld \n", (long)totRequired,
         (long)numRead);

  int fd1;
  struct iovec wiov[3];
  char str1[10];
  char str2[100];
  char str3[1000];
  ssize_t totalWrite, numWrite;
  memset(str1, 's', sizeof(str1));
  memset(str2, 's', sizeof(str2));
  memset(str3, 's', sizeof(str3));

  wiov[0].iov_base = &str1;
  wiov[0].iov_len = sizeof(str1);
  totalWrite += sizeof(str1);

  wiov[1].iov_base = &str2;
  wiov[1].iov_len = sizeof(str2);
  totalWrite += sizeof(str2);

  wiov[2].iov_base = &str3;
  wiov[2].iov_len = sizeof(str3);
  totalWrite += sizeof(str3);
  fd1 = open(argv[2], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  numWrite = my_writev(fd1, wiov, 3);
  if (numWrite == -1)
    errExit("my_writev");

  if (numWrite == numRead)
    printf("write success!\n");
  exit(EXIT_SUCCESS);
}

ssize_t my_readv(int fd, struct iovec *iovec, int count) {

  ssize_t readnum, readtol = 0;
  for (int i = 0; i < count; i++) {

    readnum = read(fd, iovec[i].iov_base, iovec[i].iov_len);
    if (readnum == -1)
      return -1;
    readtol += readnum;
  }
  return readtol;
}

ssize_t my_writev(int fd, struct iovec *iovec, int count) {

  ssize_t writetol = 0;
  for (int i = 0; i < count; i++) {

    if (write(fd, iovec[i].iov_base, iovec[i].iov_len) != iovec[i].iov_len)
      return -1;
    writetol += iovec[i].iov_len;
  }
  return writetol;
}
