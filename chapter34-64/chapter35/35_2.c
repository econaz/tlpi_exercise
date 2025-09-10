#include "tlpi_hdr.h"
#include <sched.h>

int main(int argc, char *argv[]) {
  int pol;
  struct sched_param sp;

  pol = (argv[1][0] == 'r')   ? SCHED_RR
        : (argv[1][0] == 'f') ? SCHED_FIFO
        :
#ifdef SCHED_BATCH
        (argv[1][0] == 'b') ? SCHED_BATCH
        :
#endif
#ifdef SCHED_IDLE
        (argv[1][0] == 'i') ? SCHED_IDLE
                            :
#endif
                            SCHED_OTHER;

  sp.sched_priority = getInt(argv[2], 0, "priority");

  setuid(getuid());

  if (sched_setscheduler(0, pol, &sp) == -1)
    errExit("sched_setscheduler");

  exit(EXIT_SUCCESS);
}
