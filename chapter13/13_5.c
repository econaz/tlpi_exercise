#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void read_line(char *buf, size_t readNum) {

  int i;
  int line = 0;
  char *cur;
  for (i = readNum - 1; i >= 0; i--) {

    if (buf[i] == '\n') {
      line++;
    }
  }
}
void readLine(int fd, int line) {

  size_t readNum;
  char buf[4096];
  off_t size = -4096;
  int curline = 0;
  char *curptr;

  if (lseek(fd, size, SEEK_END) == -1) {
    printf("lseek error!");
    return;
  }

  while ((readNum = read(fd, buf, size)) != -1) {
    for (int i = readNum - 1; i >= 0; i--) {
      if (buf[i] == '\n')
        curline++;
      if (curline == line)
        break;
    }
    if (curline == line)
      break;

    if (lseek(fd, size, SEEK_CUR) == -1) {
      printf("lseek error!");
      return;
    };
  }
}

int main(int argc, char *argv[]) {
  int fd;
  off_t offset;
  int line = 10;
  char *file;
  size_t readnum;
  int filesize;
  size_t bufsize = 4096;
  char buf[4096];

  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    line = atoi(argv[2]);
    file = argv[3];
  } else if (argc == 2) {
    file = argv[1];
  } else {
    printf("params error!");
    exit(EXIT_FAILURE);
  }

  fd = open(file, O_RDONLY);

  filesize = lseek(fd, 0, SEEK_END);
  if (filesize == -1) {
    printf("lseek error");
    exit(EXIT_FAILURE);
  }

  bufsize = (filesize > bufsize) ? bufsize : filesize;
  if (lseek(fd, -bufsize, SEEK_END) == -1) {
    printf("lseek error");
    exit(EXIT_FAILURE);
  }

  if ((readnum = read(fd, buf, bufsize)) == -1) {
    printf("read error");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
