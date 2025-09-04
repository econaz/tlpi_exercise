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
      errExit("getsid\n");
    printf("before sid = %ld\n", (long)sid);

    if (setpgid(0, 0) == -1)
      errExit("setpgid");
    printf("first process\n");

    if (setsid() == -1)
      errExit("setsid");

    if ((sid = getsid(pid)) == -1)
      errExit("getsid\n");

    printf("after sid = %ld\n", (long)sid);

    _exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
