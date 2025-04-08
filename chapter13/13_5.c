#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;
  off_t offset;
  int line = 10;
  char *file;
  ssize_t readNum;
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

  if ((lseek(fd, line, SEEK_END)) == -1) {
    printf("lseek error");
    exit(EXIT_FAILURE);
  }

  while ((readNum == read(fd, buf, BUF_SIZE)))
    return EXIT_SUCCESS;
}
