
#include <signal.h>

void myAbort() {

  struct sigaction sa;

  sa.sa_flags = 0;

  sigemptyset(&sa.sa_mask);
}
