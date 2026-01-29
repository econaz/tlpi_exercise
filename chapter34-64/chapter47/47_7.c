#include "semun.h"
#include "tlpi_hdr.h"
#include <errno.h>
#include <sys/sem.h>

int main(int argc, char *argv[]) {
  union semun arg;
  int maxind, ind, semid;
  struct semid_ds ds;
  struct seminfo info;

  arg.__buf = &info;
  maxind = semctl(0, 0, SEM_INFO, arg);
  if (maxind == -1)
    errExit("msgctl-SEM_INFO");

  printf("maxind = %d\n", maxind);

  arg.buf = &ds;

  for (ind = 0; ind <= maxind; ind++) {
    semid = semctl(ind, 0, SEM_STAT, arg);
    if (semid == -1) {
      if (errno != EINVAL && errno != EACCES)
        errMsg("msgctl-SEM_START");
      continue;
    }

    printf("%5d %5d %5lx\n", ind, semid, (unsigned long)ds.sem_perm.__key);
  }

  exit(EXIT_SUCCESS);
}
