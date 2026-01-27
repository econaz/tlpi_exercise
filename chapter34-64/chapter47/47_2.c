#include "curr_time.h"
#include "semun.h"
#include "tlpi_hdr.h"
#include <errno.h>
#include <error_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  union semun arg;
  struct sembuf sop;
  int semid;

  semid = semget(IPC_PRIVATE, 1, S_IWUSR | S_IRUSR);
  if (semid == -1)
    errExit("semget");

  arg.val = 1;
  if (semctl(semid, 0, SETVAL, arg) == -1)
    errExit("semctl");

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:

    printf("child started pid = %ld", (long)getpid());
    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = 1;
    if (semop(semid, &sop, 1) == -1)
      errExit("semop");

    sleep(2);

    _exit(EXIT_SUCCESS);

  default:
    printf("parent started pid = %s\n", (long)getpid());

    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = 0;
    if (semop(semid, &sop, 1) == -1)
      errExit("semop");

    printf("parent got work\n");

    if (semctl(semid, 0, IPC_RMID) == -1)
      errExit("semctl RMID");

    exit(EXIT_SUCCESS);
  }
}
