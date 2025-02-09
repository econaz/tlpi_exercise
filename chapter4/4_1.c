

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif /* ifndef  */
int main(int argc, char *argv[]) {
  size_t len;
  off_t offset;
  int fd, ap, openFlags;
  char buf[BUFF_SIZE];
  ssize_t numRead, numWritten;
  mode_t filePerms;

  if (argc == 1) {
    while (TRUE) {
      numRead = read(STDIN_FILENO, buf, BUFF_SIZE);
      if (numRead == -1)
        errExit("read");

      numWritten = write(STDOUT_FILENO, buf, numRead);
      if (numWritten != numRead)
        errExit("wirte");
    }
  } else if (argc == 2 || argc == 3) {

    openFlags = O_CREAT | O_RDWR;

    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if (argc == 2) {
      openFlags = openFlags | O_TRUNC;
      fd = open(argv[1], openFlags, filePerms);
    }
    if (argc == 3) {
      if (strcmp(argv[1], "-a") == 0) {
        openFlags = openFlags | O_APPEND;
        fd = open(argv[2], openFlags, filePerms);
      } else {
        cmdLineErr("arg error");
      }
    }
    if (fd < 0)
      errExit("open file");

    while (TRUE) {
      numRead = read(STDIN_FILENO, buf, BUFF_SIZE);
      if (numRead == -1)
        errExit("read");
      numWritten = write(fd, buf, numRead);
      if (numWritten != numRead)
        errExit("write");
      if (argc == 3)
        printf("%s: append wrote %ld bytes\n", argv[1], (long)numWritten);
      else
        printf("%s wrote %ld bytes\n", argv[1], (long)numWritten);
    }
  } else {
    cmdLineErr("args erros");
  }
  return EXIT_SUCCESS;
}
