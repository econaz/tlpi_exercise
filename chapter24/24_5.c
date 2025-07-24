#include "tlpi_hdr.h"
#include <signal.h>
#include <time.h>

#define SYNC_SIG SIGUSR1
#define BUF_SIZE 1000

static void handler(int sig) {}

char *currTime(const char *format) {
  static char buf[BUF_SIZE];
  time_t t;
  size_t s;
  struct tm *tm;

  t = time(NULL);

  tm = localtime(&t);

  if (tm == NULL)
    return NULL;

  s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

  return (s == 0) ? NULL : buf;
}
int main(int argc, char *argv[]) {
  pid_t childPid;
  sigset_t blockMask, origMask, emptyMask;
  struct sigaction sa;

  setbuf(stdout, NULL);

  sigemptyset(&blockMask);
  sigaddset(&blockMask, SYNC_SIG);
  if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
    errExit("sigprocmask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = handler;

  if (sigaction(SYNC_SIG, &sa, NULL) == -1)
    errExit("sigaction");

  switch (childPid = fork()) {
  case -1:
    errExit("fork");
  case 0:
    printf("[%s %ld] Child about to wait for signal\n", currTime("%T"),
           (long)getpid());
    sigemptyset(&emptyMask);
    if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
      errExit("sigsuspend");
    printf("[%s %ld] Child got signal\n", currTime("%T"), (long)getpid());

    if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
      errExit("sigprocmask");

    _exit(EXIT_SUCCESS);
  default:
    printf("[%s %ld] Parent started - doing some work\n", currTime("%T"),
           (long)getpid());

    // sleep(2);
    printf("[%s %ld] Parent about to signal child\n", currTime("%T"),
           (long)getpid());
    if (kill(childPid, SYNC_SIG) == -1)
      errExit("kill");

    exit(EXIT_SUCCESS);
  }
}
