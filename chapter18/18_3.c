

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
  char bufN[BUF_SIZE];
  ssize_t numBytes;
  int fd;

  if (stat(name, &statbuf) == -1)
    return NULL;

  numBytes = readlink(name, buf, BUF_SIZE - 1);
  if (numBytes == -1) {
    printf("error readlink");
    return NULL;
  }
  if (buf[0] == '/') {
    snprintf(resolved, numBytes, "%s", buf);
    return resolved;
  } else {
    snprintf(bufN, BUF_SIZE, "%s", name);
    fd = open(dirname(bufN), O_RDONLY);
    if (fd == -1)
      return NULL;
    if (fchdir(fd) == -1)
      return NULL;
    if (!getcwd(resolved, BUF_SIZE))
      return NULL;
    strcat(resolved, "/");
    strcat(resolved, buf);
    close(fd);
    return resolved;
  }
  return NULL;
}

int main(int argc, char *argv[]) {

  char str[BUF_SIZE];
  myrealpath(argv[1], str);
  printf("%s-----> %s", argv[1], str);
}
