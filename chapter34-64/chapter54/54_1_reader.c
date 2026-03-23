#include "54_1.h"
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>


int main(int argc,char *argv[]){


  int xfrs,bytes,shmid;
  struct shmseg *shmp;

  
  shmid = shm_open(SHM_NAME, O_RDWR, OBJ_PERMS);
  if (shmid == -1)
    errExit("shm_open");

  if (ftruncate(shmid, sizeof(struct shmseg)) == -1)
    errExit("ftruncate");

  shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ, MAP_SHARED, shmid, 0);
  if ((void *)shmp == MAP_FAILED)
    errExit("mmap");

  rSem = sem_open(READ_SEM , O_RDWR,OBJ_PERMS);
  wSem = sem_open(WRITE_SEM , O_RDWR,OBJ_PERMS);

  if (close(shmid) == -1)
    errExit("close");

  printf("out");
  
  for (xfrs = 0,bytes = 0;;xfrs++){

    if (sem_wait(rSem) == -1)
      errExit("sem_wait");

    if (shmp->cnt == 0)
      break;
    bytes += shmp->cnt;

    if (write(STDOUT_FILENO, shmp->buf,shmp->cnt) != shmp->cnt)
      fatal("write");

    if (sem_post(wSem) == -1)
      errExit("sem_post");
  }

  if (sem_post(wSem) == -1)
    errExit("sem_post");
  
  fprintf(stderr, "Receive %d bytes (%d xfrs)\n",bytes,xfrs);

  exit(EXIT_SUCCESS);

}
