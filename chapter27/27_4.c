#include "tlpi_hdr.h"
#include <error_functions.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int childPid = fork();
  int status;
  if (childPid == -1)
    errExit("fork1");
  if (childPid == 0) {
    switch (fork()) {
    case -1:
      errExit("fork2");
    case 0:
      /*-----------*/
      exit(EXIT_SUCCESS);
    default:
      exit(EXIT_SUCCESS);
    }
  }
  if (waitpid(childPid, &status, 0) == -1)
    errExit("waitpid");

  return EXIT_SUCCESS;
}

// 孙子进程会被init进程接管,不会成为孤儿进程。
