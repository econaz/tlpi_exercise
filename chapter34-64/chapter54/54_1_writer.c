#include "54_1.h"
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>


int main(int argc,char *argv[]){

  
  int xfrs,bytes,shmid;
  struct shmseg *shmp;


  printf("test1\n");

  sem_unlink(WRITE_SEM);
  sem_unlink(READ_SEM);
  shm_unlink(SHM_NAME);


  
  wSem = sem_open(WRITE_SEM, O_RDWR | O_CREAT | O_EXCL,OBJ_PERMS,1);
  if (wSem == SEM_FAILED)
    errExit("sem_open1");

  rSem = sem_open(READ_SEM,O_RDWR | O_CREAT | O_EXCL,OBJ_PERMS,0);
  if (rSem == SEM_FAILED)
    errExit("sem_open2");


  printf("test\n");
  shmid = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL,OBJ_PERMS);
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

  if (shm_unlink(SHM_NAME) == -1)
    errExit("shm_unlink");
  
  fprintf(stderr, "Send %d bytes (%d xfrs)\n",bytes,xfrs);

  if (sem_unlink(READ_SEM) == -1)
    errExit("sem_destroy");
  if (sem_unlink(WRITE_SEM) == -1)
    errExit("sem_destory");

  
  exit(EXIT_SUCCESS);

}
