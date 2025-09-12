#include "tlpi_hdr.h"
#include <math.h>
#include <sched.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

void handle(pid_t pid) {
  struct tms startTime, endTime;
  clock_t clockTime;
  double time;
  static long clockTicks = 0;
  long cost;
  int flag = 0;

  if (clockTicks == 0) {
    clockTicks = sysconf(_SC_CLK_TCK);
    if (clockTicks == -1)
      errExit("sysconf");
  }
  if (times(&startTime) == -1)
    errExit("times");

  for (;;) {
    if (times(&endTime) == -1)
      errExit("times");

    cost = (endTime.tms_stime + endTime.tms_utime) -
           (startTime.tms_stime + startTime.tms_utime) / clockTicks;

    // todo
    if (cost == 3.0)
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
    _exit(EXIT_SUCCESS);
  default:

    handle(getpid());
    if (wait(&status) == -1)
      errExit("wait");
  }

  return EXIT_SUCCESS;
}
