#include "tlpi_hdr.h"
#include <libgen.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMD_SIZE 200
static volatile int flag = 0;
static void sigchldHandler(int sig) { flag = 1; }

int main(int argc, char *argv[]) {
  char cmd[CMD_SIZE];
  pid_t childPid;
  struct sigaction sa;
  sigset_t blockMask;

  setbuf(stdout, NULL);

  printf("parent PID=%ld\n", (long)getpid());

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigchldHandler;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    errExit("sigaction");

  // sigemptyset(&blockMask);
  // sigaddset(&blockMask, SIGCHLD);
  // if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
  //   errExit("sigprocmask");

  switch (childPid = fork()) {
  case -1:
    errExit("fork");
  case 0:
    printf("Child (PID=%ld) exiting\n", (long)getpid());
    exit(EXIT_SUCCESS);
  default:
    while (!flag)
      pause();
    snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
    cmd[CMD_SIZE - 1] = '\0';
    system(cmd);

    if (kill(childPid, SIGKILL) == -1)
      errMsg("kill");
    sleep(3);
    printf("After sending SIGKILL to zombie (PID=%ld):\n", (long)childPid);
    system(cmd);

    exit(EXIT_SUCCESS);
  }
}
