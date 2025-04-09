

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
//
// because O_APPEND,the two file is not same;
int main(int argc, char *argv[]) {

  int fd;
  off_t off_t;
  int append = 1, param = O_RDWR | O_APPEND;

  if (argc > 4 || argc == 1 || strcmp(argv[1], "--help") == 0)
    usageErr("use param error!");

  if (argv[2] != NULL)
    append = atoi(argv[2]);
  if (argv[3] != NULL && strcmp(argv[3], "x") == 0)
    param = O_RDWR;
  if (argv[3] != NULL && strcmp(argv[3], "x") != 0)
    usageErr("param argv[3] error!");

  fd = open(argv[1], param, S_IRUSR | S_IWUSR);
  if (fd == -1)
    errExit("open");
  char a = 'a';
  for (int i = 0; i < append; i++) {
    if (lseek(fd, 0, SEEK_END) == -1)
      errExit("lseek");
    if (write(fd, &a, 1) != 1)
      errExit("write");
  }
  exit(EXIT_SUCCESS);
}
