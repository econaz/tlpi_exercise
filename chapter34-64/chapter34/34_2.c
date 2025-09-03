
#include "tlpi_hdr.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t gpid;
  pid_t pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    printf("child sleep\n");
    sleep(10);

    execl("/usr/bin/sleep", "sleep", "10", (char *)NULL);
    _exit(EXIT_SUCCESS);
  default:
    break;
  }

  if ((gpid = getpgid(pid)) == -1)
    errExit("getpgid");

  printf("before pgid = %ld\n", (long)gpid);

  if (setpgid(pid, pid) == -1) {
    printf("cant modify pgid\n");
  }
  printf("after pgid = %ld\n", (long)gpid);

  sleep(10);
  if (setpgid(pid, pid) == -1)
    errExit("setpgid");

  if ((gpid = getpgid(pid)) == -1)
    errExit("getpgid");

  printf("After pgid = %ld", (long)gpid);
  return EXIT_SUCCESS;
}
