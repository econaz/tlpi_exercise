#include <bits/time.h>
#include <bits/types/timer_t.h>
#include <csignal>
#include <cstdlib>
#include <error_functions.h>
#include <string.h>
#define _POSIX_C_SOURCE 199309
#include "tlpi_hdr.h"
#include <signal.h>
#include <time.h>

static void handler(int sig) { return; }
int main(int argc, char *argv[]) {
  timer_t t;
  struct itimerspec val;
  struct sigaction sa;

  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;

  if (sigaction(SIGINT, &sa, NULL) == -1)
    errExit("sigaction");

  if (timer_create(CLOCK_REALTIME, NULL, &t) == -1)
    errExit("timer_create");

  if (timer_settime(&t, 0, &val, NULL) == -1)
    errExit("timer_settime");

  return 0;
}
