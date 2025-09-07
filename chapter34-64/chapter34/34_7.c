#include "tlpi_hdr.h"
#include <signal.h>
#include <unistd.h>

void handler(int sig) { printf("got sig %d\n", sig); }

int main(int argc, char *argv[]) {
  pid_t pid;
  pid = fork();
  struct sigaction sa;
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    sleep(10);
#ifdef HANDLER
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
      errExit("sigaction");
#endif
  default:
    exit(EXIT_SUCCESS);
  }
  return EXIT_SUCCESS;
}
