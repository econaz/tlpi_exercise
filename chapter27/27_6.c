#include "tlpi_hdr.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void handler(int sig) { printf("got sig %d\n", sig); }

int main(int argc, char *argv[]) {

  struct sigaction sa;
  sigset_t block;
  int status;
  char cmd[20] = "ps -a";

  sa.sa_flags = 0;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);

  sigaddset(&block, SIGCHLD);

  sigprocmask(SIG_BLOCK, &block, NULL);
  sigaction(SIGCHLD, &sa, NULL);

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    _exit(EXIT_SUCCESS);
  default:
    system(cmd);
    sleep(3);
    if (wait(&status) == -1)
      errExit("wait");

    system(cmd);
    sleep(3);

    sigemptyset(&block);
    sigprocmask(SIG_SETMASK, &block, NULL);
  }
}
