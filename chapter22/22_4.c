#include <bits/types/sigset_t.h>
#include <csignal>
#include <signal.h>
#include <string.h>

typedef void (*sighandler_t)(int);
#define SIG_HOLD ((sighandler_t)4)

int sighold(int sig) {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, sig);
  return sigprocmask(SIG_BLOCK, &set, NULL);
}
sighandler_t sigset(int sig, sighandler_t handler) {
  struct sigaction sa;
  if (handler == SIG_HOLD) {
    sighold(sig);
    sigaction(sig, NULL, &sa);
    return sa.sa_handler;
  }
  struct sigaction act;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = handler;
  sigaction(sig, &act, &sa);
  return sa.sa_handler;
}
int sigrelse(int sig) {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, sig);
  return sigprocmask(SIG_UNBLOCK, &set, NULL);
}
int sigignore(int sig) {
  if (signal(sig, SIG_IGN) == SIG_ERR)
    return -1;
  return 0;
}
int sigpause(int sig) {
  sigset_t set;
  if (sigprocmask(SIG_BLOCK, NULL, &set) == -1)
    return -1;
  sigdelset(&set, sig);
  return sigsuspend(&set);
}
static void handler(int sig) {}
int main(int argc, char *argv[]) {
  sigset(SIGQUIT, handler);
  sighold(SIGTERM);
  sigrelse(SIGTERM);
  sigignore(SIGTERM);
  sigpause(SIGTERM);
}
