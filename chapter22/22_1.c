

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int sig) {
  if (sig == SIGCONT) {
    printf("receive sig SIGCONT\n");
  }
}

int main(int argc, char *argv[]) {
  struct sigaction sa;

  sa.sa_flags = 0;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);

  sigaction(SIGCONT, &sa, NULL);

  while (1) {
    // pause();
  }
  exit(EXIT_SUCCESS);
}
