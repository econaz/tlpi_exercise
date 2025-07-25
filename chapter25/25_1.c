#include "tlpi_hdr.h"
#include <stdio.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  int status;
  pid_t pid = fork();

  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    exit(-1);

  default:
    wait(&status);
  }

  printf("p %d\n", status);
  if (WIFEXITED(status))
    printf("%d\n", WEXITSTATUS(status));

  exit(EXIT_SUCCESS);
}
// -1
//
//
