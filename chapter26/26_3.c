#include "tlpi_hdr.h"
#include <bits/types/idtype_t.h>
#include <bits/types/siginfo_t.h>
#include <error_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void printWaitStatus(const char *msg, const siginfo_t *info) {

  if (msg != NULL)
    printf("%s", msg);
  int status = info->si_status;
  if (info->si_code == CLD_EXITED)
    printf("child exited,status=%d\n", status);
  else if (info->si_code == CLD_KILLED)
    printf("child killed by signal %d\n ", status >> 7);
  else if (info->si_code == CLD_STOPPED)
    printf("child stop by signal %d\n", status);
  else if (info->si_code == CLD_CONTINUED)
    printf("child countined\n");
  else
    printf("child\n");
}
int main(int argc, char *argv[]) {
  siginfo_t info;
  pid_t childPid;

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [exit-status]\n", argv[0]);

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    printf("Child started with PID = %ld\n", (long)getpid());
    if (argc > 1)
      exit(getInt(argv[1], 0, "exit-status"));
    else
      for (;;)
        pause();
    printf("test");
    exit(EXIT_FAILURE);

  default:
    for (;;) {

      childPid = waitid(P_ALL, 0, &info, WEXITED);
      if (childPid == -1)
        errExit("waitid");
      printf("child exit with %d\n", info.si_status);
      printWaitStatus(NULL, &info);
      if (info.si_code == CLD_KILLED || info.si_code == CLD_EXITED)
        exit(EXIT_SUCCESS);
    }
  }
}
