#include "tlpi_hdr.h"
#include <error_functions.h>
#include <linux/capability.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/capability.h>

// cap_get_proc
static int modifyCap(int capability, int setting) {
  cap_t caps;
  cap_value_t capList[1];

  caps = cap_get_proc();
  if (caps == NULL)
    return -1;

  capList[0] = capability;
  if (cap_set_flag(caps, CAP_EFFECTIVE, capList, setting) == -1) {
    cap_free(caps);
    return -1;
  }

  if (cap_set_proc(caps) == -1) {
    cap_free(caps);
    return -1;
  }

  if (cap_free(caps) == -1)
    return -1;

  return 0;
}

static int raiseCap(int capability) { return modifyCap(capability, CAP_SET); }

static int dropAllCaps(void) {

  cap_t empty;
  int s;

  empty = cap_init();
  if (empty == NULL)
    return -1;

  s = cap_set_proc(empty);
  if (cap_free(empty) == -1)
    return -1;
  return s;
}
int main(int argc, char *argv[]) {
  int j, pol;
  struct sched_param sp;
  if (argc < 3 || strchr("rfo", argv[1][0]) == NULL)
    usageErr("%s policy priority [pid..]\n"
             "    policy is 'r' (RR),'f' (FIFO),"
#ifdef SCHED_BATCH
             "'b'(BATCH),"
#endif
#ifdef SCHED_IDELE
             "'i' (IDLE),"
#endif
             "or 'o' (OTHER)\n",
             argv[0]);
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

  if (raiseCap(CAP_SYS_NICE) == -1)
    errExit("raiseCap () failed");

  for (j = 3; j < argc; j++)
    if (sched_setscheduler(getLong(argv[j], 0, "pid"), pol, &sp) == -1)
      errExit("sched_setscheduler");

  if (dropAllCaps() == -1)
    errExit("dropAllCaps() failed");

  exit(EXIT_SUCCESS);
}
