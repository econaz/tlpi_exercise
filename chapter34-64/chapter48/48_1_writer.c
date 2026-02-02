#include "47_5.h"
#include "48_1.h"
#include "semun.h"
#include <error_functions.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {

  int semid, shmid, bytes, xfrs;
  struct shmseg *shmp;
  union semun dummy;

  semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
  if (semid == -1)
    errExit("semget");

  if (setEventFlag(semid, WRITE_SEM) == -1)
    errExit("setEventFlag");
  if (clearEventFlag(semid, READ_SEM) == -1)
    errExit("clearEventFlag");

  shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
  if (shmid == -1)
    errExit("shmget");

  shmp = shmat(shmid, NULL, 0);
  if (shmp == (void *)-1)
    errExit("shmp");
}
