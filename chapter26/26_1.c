#include "tlpi_hdr.h"
#include <unistd.h>

int main(int argc, char *argv[]) {

  pid_t pid = fork();
  switch (pid) {

  case -1:
    errExit("fork");
  case 0:
    sleep(10);
    printf("my parent pid is %d\n", getppid());
  default:
    exit(EXIT_SUCCESS);
  }
  return EXIT_SUCCESS;
}
