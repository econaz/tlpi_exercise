
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void handler(int sig) {
  printf("in\n");
  sleep(10);
  printf("out\n");
}
int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_flags = SA_NODEFER;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);

  printf("pid is %ld\n", (long)getpid());

  if (sigaction(SIGTERM, &act, NULL) == -1) {
    printf("sigaction");
    exit(EXIT_FAILURE);
  }
  while (1) {
  }

  exit(EXIT_SUCCESS);
}
