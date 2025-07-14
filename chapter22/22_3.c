#include "tlpi_hdr.h"
#include <signal.h>
#include <unistd.h>

static void handler(int sig) {}

#define TESTSIG SIGUSR1

int main(int argc, char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s num-sigs\n", argv[0]);

  int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

  // struct sigaction sa;
  // sigemptyset(&sa.sa_mask);
  // sa.sa_flags = 0;
  // sa.sa_handler = handler;
  //
  // if (sigaction(TESTSIG, &sa, NULL) == -1)
  //   errExit("sigaction");

  sigset_t blockedMask;
  sigemptyset(&blockedMask);
  sigaddset(&blockedMask, TESTSIG);
  if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
    errExit("sigprocmask");

  pid_t childPid = fork();

  switch (childPid) {
  case -1:
    errExit("fork");
  case 0:
    for (int scnt = 0; scnt < numSigs; scnt++) {
      if (kill(getppid(), TESTSIG) == -1)
        errExit("kill");
      if (sigwaitinfo(&blockedMask, NULL) == -1 && errno != EINTR)
        errExit("sigwaitinfo");
    }
    exit(EXIT_SUCCESS);
  default:
    for (int scnt = 0; scnt < numSigs; scnt++) {

      if (sigwaitinfo(&blockedMask, NULL) == -1 && errno != EINTR)
        errExit("sigwaitinfo");
      if (kill(childPid, TESTSIG) == -1)
        errExit("kill");
    }
    exit(EXIT_SUCCESS);
  }
}
