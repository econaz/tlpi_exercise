#define _GNU_SOURCE
#include "signal_functions.h"
#include "tlpi_hdr.h"
#include <signal.h>
#include <string.h>

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
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

int printSigMask(FILE *of, const char *msg) {
  sigset_t currMask;

  if (msg != NULL)
    fprintf(of, "%s", msg);

  if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
    return -1;

  printSigset(of, "\t\t", &currMask);
  return 0;
}

int printPendingSigs(FILE *of, const char *msg) {
  sigset_t pendingSigs;

  if (msg != NULL)
    fprintf(of, "%s", msg);

  if (sigpending(&pendingSigs) == -1)
    return -1;

  printSigset(of, "\t\t", &pendingSigs);
  return 0;
}
