#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {

  pid_t pid;
  char buf[1024];
  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    sleep(3);
    if (read(STDIN_FILENO, buf, 1024) == -1)
      errExit("read");

  default:
    exit(EXIT_SUCCESS);
  }
}
