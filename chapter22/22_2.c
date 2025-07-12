#include <bits/types/sigset_t.h>
#include <cstddef>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int sig) { printf("GOT sig %d\n", sig); }

int main(int argc, char *argv[]) {

  struct sigaction sa, oldsa;
  sigset_t block;
  int i;

  sigfillset(&block);
  sa.sa_flags = 0;
  sa.sa_handler = handler;

  sigfillset(&sa.sa_mask);

  if (sigprocmask(SIG_SETMASK, &block, NULL) == -1) {
    printf("sigprocmask error");
    exit(EXIT_FAILURE);
  }
  for (i = 1; i < 32; i++) {
    if (i == SIGKILL || i == SIGSTOP)
      continue;

    if (sigaction(i, &sa, NULL) == -1) {
      printf("sigaction error1");
      exit(EXIT_FAILURE);
    }
  }

  for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
    if (i == SIGKILL || i == SIGSTOP)
      continue;

    if (sigaction(i, &sa, NULL) == -1) {
      printf("sigaction error2");
      exit(EXIT_FAILURE);
    }
  }

  for (i = 32; i > 0; i--) {
    if (i == SIGKILL || i == SIGSTOP)
      continue;
    raise(i);
  }
  for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
    if (i == SIGKILL || i == SIGSTOP)
      continue;
    raise(i);
  }

  sigemptyset(&block);

  if (sigprocmask(SIG_SETMASK, &block, NULL) == -1) {
    printf("sigprocmask error");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
