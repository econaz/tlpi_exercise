

#include "tlpi_hdr.h"
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int i = 0;
  int status;
  pid_t cpid;

  while (i <= 100000) {
    // cpid = fork(); //9.18s user 5.50s system 101% cpu 14.406 total
    cpid = vfork(); // 0.90s user 0.98s system 112% cpu 1.676 total

    switch (cpid) {
    case -1:
      errExit("fork");
    case 0:
      _exit(EXIT_SUCCESS);
    default:
      if (wait(&status) == -1)
        errExit("wait");
      printf("time %ds got pid = %d\n", i, cpid);
      i++;
      continue;
    }
  }
}
