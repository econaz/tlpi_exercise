#include <fcntl.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096

int read_tail(char *file, int line) {
  int pos = 0, fd, readline, i;
  off_t filesize, readsize;
  ssize_t readnum;
  char buf[BUF_SIZE];

  fd = open(file, O_RDONLY);
  if (fd == -1) {
    printf("open error");
    return -1;
  }

  // get file size and position file end
  filesize = lseek(fd, 0, SEEK_END);
  if (filesize == -1) {
    printf("lseek error1");
    return -1;
  }

  while (abs(pos) < filesize && readline < line + 1) {

    // cal remaind file size
    readsize =
        (filesize - abs(pos) > BUF_SIZE) ? BUF_SIZE : filesize - abs(pos);

    if (lseek(fd, -readsize, SEEK_CUR) == -1) {
      printf("lseek error2");
      exit(EXIT_FAILURE);
    }

    if ((readnum = read(fd, buf, readsize)) == -1) {
      printf("read error");
      exit(EXIT_FAILURE);
    }

    for (i = readnum - 1; i >= 0; i--) {
      if (buf[i] == '\n')
        readline++;
      // read line + one,the posistion add one;
      if (readline == line + 1) {
        pos = pos - (readnum - i) + 1;
        break;
      }
    }
    if (readline < line + 1) {
      pos -= readnum;
    }
  }
  if (lseek(fd, pos, SEEK_END) == -1) {
    printf("lseek error3");
    return -1;
  }
  while ((readnum = read(fd, buf, BUF_SIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, readnum) == -1) {
      printf("write error");
      return -1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int line = 10;
  char *file;

  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    line = atoi(argv[2]);
    file = argv[3];
  } else if (argc == 2) {
    file = argv[1];
  } else {
    printf("params error!");
    exit(EXIT_FAILURE);
  }

  if (read_tail(file, line) == -1) {
    printf("read_tail error!");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
