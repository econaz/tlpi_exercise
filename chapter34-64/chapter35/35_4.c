#define _GNU_SOURCE
#include "tlpi_hdr.h"
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipefd[2];
  char buf[1024];
  pid_t pid;

#ifdef ONECPU

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(1, &set);
#endif

  if (pipe(pipefd) == -1)
    errExit("pipe");

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");

  case 0:
#ifdef ONECPU
    sched_setaffinity(getpid(), CPU_SETSIZE, &set);
    printf("test\n");
#endif

    for (int i = 0; i < 1000000; i++) {
      close(pipefd[1]);
      read(pipefd[0], buf, sizeof(buf));
      printf("%s %d\n", buf, i);
      close(pipefd[0]);
    }

  default:
#ifdef ONECPU
    sched_setaffinity(getpid(), CPU_SETSIZE, &set);
#endif
    for (int j = 0; j < 1000000; j++) {
      close(pipefd[0]);
      char *message = "hello child";

      write(pipefd[1], message, 11);
      close(pipefd[1]);
    }
  }

  exit(EXIT_SUCCESS);
}
