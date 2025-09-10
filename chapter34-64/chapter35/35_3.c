#include "tlpi_hdr.h"
#include <sched.h>
#include <sys/times.h>
#include <unistd.h>

void handle(pid_t pid) {
  struct tms tms;

  for (;;) {
    if (times(&tms) == -1)
      errExit("times");
    if (tms.tms_stime)
  }
}

int main(int argc, char *argv[]) {
  struct sched_param sp;
  pid_t pid;

  if (sched_setscheduler(0, SCHED_FIFO, 0) == -1)
    errExit("sched_setscheduler");

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
  }

  return EXIT_SUCCESS;
}
