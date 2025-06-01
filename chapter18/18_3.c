

#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE PATH_MAX

char *myrealpath(const char *name, char *resolved) {

  struct stat statbuf;
  char buf[BUF_SIZE];
  ssize_t numBytes;

  if (lstat(name, &statbuf) == -1)
    return NULL;

  numBytes = readlink(name, buf, BUF_SIZE - 1);
  if (numBytes == -1) {
    printf("error readlink\n");
    exit(EXIT_FAILURE);
  }

  buf[numBytes] = '\0';
  printf("%s\n", buf);
  return NULL;
}

int main(int argc, char *argv[]) {

  char str[BUF_SIZE];
  myrealpath(argv[1], str);
}
