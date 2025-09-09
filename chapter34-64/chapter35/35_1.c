#include "tlpi_hdr.h"
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

int my_nice(int incr) {

  int nice;

  errno = 0;
  nice = getpriority(PRIO_PROCESS, 0);
  if (nice == -1 && errno != 0)
    return -1;

  if (setpriority(PRIO_PROCESS, 0, nice + incr) == -1)
    return -1;
  return 0;
}

int main(int argc, char *argv[]) {

  int incr, nice;
  pid_t pid;

  pid = getLong(argv[1], 0, "pid");
  incr = getInt(argv[2], 0, "incr");

  errno = 0;
  nice = getpriority(PRIO_PROCESS, pid);
  if (nice == -1 && errno != 0)
    errExit("getpriority");
  printf("before pid = %ld nice = %d\n",
         (pid == 0) ? (long)getpid() : (long)pid, nice);

  if (my_nice(incr) == -1)
    errExit("my_nice");

  errno = 0;
  nice = getpriority(PRIO_PROCESS, pid);
  if (nice == -1 && errno != 0)
    errExit("getpriority");
  printf("after pid = %ld nice = %d\n", (pid == 0) ? (long)getpid() : (long)pid,
         nice);
  exit(EXIT_SUCCESS);
}
