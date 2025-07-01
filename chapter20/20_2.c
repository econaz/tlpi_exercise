#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct sigaction act;

  printf("pid is %ld\n", (long)getpid());
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;
  if (sigaction(SIGTERM, &act, NULL) == -1) {

    printf("sigaction");
    exit(EXIT_FAILURE);
  }

  while (1) {
  }

  exit(EXIT_SUCCESS);
}
