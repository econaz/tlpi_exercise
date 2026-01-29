#include "curr_time.h"
#include "semun.h"
#include "tlpi_hdr.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>

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

    printf("child started pid = %ld,time:%s\n ", (long)getpid(),
           currTime("%T"));
    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = -1;
    if (semop(semid, &sop, 1) == -1)
      errExit("semop");

    sleep(2);

    _exit(EXIT_SUCCESS);

  default:
    printf("parent wait for child pid = %ld\n,time:%s\n", (long)getpid(),
           currTime("%T"));

    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = 0; // wait for child set value = 0;

    if (semop(semid, &sop, 1) == -1)
      errExit("semop");

    printf("parent got work wait:%s\n", currTime("%T"));

    if (semctl(semid, 0, IPC_RMID) == -1)
      errExit("semctl RMID");

    exit(EXIT_SUCCESS);
  }
}
