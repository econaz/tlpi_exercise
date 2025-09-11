#include "tlpi_hdr.h"
#include <error_functions.h>
#include <math.h>
#include <sched.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

#undef max
#undef min

void handle(pid_t pid) {
  struct tms tms;
  clock_t clockTime;
  double time;
  static long clockTicks = 0;
  if (clockTicks == 0) {
    clockTicks = sysconf(_SC_CLK_TCK);
    if (clockTicks == -1)
      errExit("sysconf");
  }

  for (;;) {
    if (times(&tms) == -1)
      errExit("times");
    time = (double)(tms.tms_stime + tms.tms_utime) / clockTime;
    if (fmod(time, 0.25) == 0.0) {
      printf("%d cost 0.25s\n", pid);
    }
    if (fmod(time, 1.0) == 0.0) {
      sched_yield();
      printf("%d cost 1s\n", pid);
    }
    if ((time - 3) < 1e-6)
      break;
  }
}

int main(int argc, char *argv[]) {
  struct sched_param sp;
  pid_t pid;
  int pol, status;
  pol = SCHED_FIFO;

  if (sched_getparam(0, &sp) == -1)
    errExit("sched_getparam");

  printf("%d\n", sp.sched_priority);

  sp.sched_priority = 2;
  if (sched_setscheduler(0, pol, &sp) == -1)
    errExit("sched_setscheduler");

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:

    handle(getpid());
  default:

    handle(getpid());
    // if (wait(&status) == -1)
    //   errExit("wait");
  }

  return EXIT_SUCCESS;
}
