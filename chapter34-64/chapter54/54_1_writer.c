#include "54_1.h"
#include <error_functions.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>


int main(int argc,char *argv[]){

  
  int xfrs,bytes,shmid;
  struct shmseg *shmp;


  printf("test1\n");

  wSem = sem_open(WRITE_SEM, O_RDWR,S_IRUSR | S_IWUSR,1);
  if (wSem == SEM_FAILED)
    errExit("sem_open");

  rSem = sem_open(READ_SEM, O_RDWR,S_IRUSR | S_IRUSR,0);
  if (rSem == SEM_FAILED)
    errExit("sem_open");


  printf("test\n");
  shmid = shm_open(SHM_NAME, O_RDWR, 0);
  if (shmid == -1)
    errExit("shm_open");

  if (ftruncate(shmid, sizeof(struct shmseg)) == -1)
    errExit("ftruncate");

  shmp = mmap(NULL, sizeof(struct shmseg), PROT_WRITE | PROT_READ, MAP_SHARED, shmid
              , 0);

  if ((void *)shmp == MAP_FAILED)
    errExit("mmap");

  if (close(shmid) == -1)
    errExit("close");

    
  for (xfrs = 0,bytes = 0;;xfrs++,bytes += shmp->cnt){

    if (sem_wait(wSem) == -1)
      errExit("sem_wait");

    shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);

    if (shmp->cnt == -1)
      errExit("read");

    if (sem_post(rSem) == -1)
      errExit("sem_post");

    if (shmp->cnt == 0)
      break;
  }

  if (sem_wait(wSem) == -1)
    errExit("sem_wait");

  fprintf(stderr, "Send %d bytes (%d xfrs)\n",bytes,xfrs);
  exit(EXIT_SUCCESS);

}
