#include "tlpi_hdr.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    abort();
  default:
    printf("ppid\n");
  }
  exit(EXIT_SUCCESS);
}
