#include "48_1.h"
#include <error_functions.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {

  int semid, shmid, xfrs, bytes;
  struct shmseg *shmp;

  semid = semget(SEM_KEY, 0, 0);
  if (semid == -1)
    errExit("semget");

  shmid = shmget(SHM_KEY, 0, 0);
  if (shmid == -1)
    errExit("shmget");

  shmp = shmat(shmid, NULL, ) return EXIT_SUCCESS;
}
