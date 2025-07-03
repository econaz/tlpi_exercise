

#include <csignal>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void handler(int sig) { if (sig == SIGTERM) }
int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;

  act.sa_flags |= SA_RESETHAND;

  if (sigaction(SIGTERM, &act, NULL) == -1) {
    printf("sigaction");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
