#include "tlpi_hdr.h"
#include <error_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t sid, pid;
  pid = fork();

  switch (pid) {
  case -1:
    errExit("fork");

  case 0:

    if ((sid = getsid(pid)) == -1)
      errExit("getsid");
    printf("before sid = %ld\n", (long)sid);
  }
  return EXIT_SUCCESS;
}
