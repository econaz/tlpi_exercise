

#include "tlpi_hdr.h"
#include <cstdio>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

static void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {

  int sig, cnt;

  cnt = 0;
  for (sig = 1; sig < NSIG; sig++) {
    if (sigismember(sigset, sig)) {
      cnt++;
      fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
    }
  }
  if (cnt == 0)
    fprintf(of, "%s<empty signal set>\n", prefix);
}

static int printPendingSigs(FILE *of, const char *msg) {

  sigset_t pendingSigs;

  if (msg != NULL)
    fprintf(of, "%s", msg);

  if (sigpending(&pendingSigs) == -1)
    return -1;

  printSigset(of, "\t\t", &pendingSigs);
  return 0;
}

static void *threadFunc(void *arg) { printPendingSigs(stdin, NULL); }

int main(int argc, char *argv[]) {
  pthread_t phr, phr1;

  int s;
  sigset_t block;
  sigfillset(&block);

  s = pthread_sigmask(SIG_SETMASK, &block, NULL);
  if (s != 0)
    errExitEN(s, "pthread_sigmask");

  s = pthread_create(&phr, NULL, threadFunc, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");

  s = pthread_create(&phr1, NULL, threadFunc, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");

  pthread_kill(phr, SIGINT);
  pthread_kill(phr1, SIGINT);

  exit(EXIT_SUCCESS);
}
