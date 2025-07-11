
#include <bits/types/sigset_t.h>
#include <signal.h>
#include <string.h>

void myAbort() {

  struct sigaction sa;
  sigset_t mask;
  sigaddset(&mask, SIGABRT);
  sigprocmask(SIG_UNBLOCK, &mask, NULL);
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  raise(SIGABRT);

  sigaction(SIGABRT, &sa, NULL);
}
