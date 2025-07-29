#include "tlpi_hdr.h"
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int stat;

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    switch (fork()) {
    case -1:
      errExit("fork");
    case 0:
      sleep(10);
      printf("%d\n", getppid());
      sleep(1);
      printf("%d\n", getppid());
    default:
      sleep(10);
      exit(EXIT_SUCCESS);
    }
  default:
    sleep(12);
    wait(&stat);
  }
}
