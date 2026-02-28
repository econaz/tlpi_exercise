#include "semun.h"
#include "svshm_xfr.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
  int semid,xfrs,bytes;
  struct shmseg *shmp;
  int fd;
  char *addr;
  union semun dummy;

  semid = semget(SEM_KEY,2,IPC_CREAT | OBJ_PERMS);
  if (semid == -1)
    errExit("semget");

  if (initSemAvailable(semid,WRITE_SEM) == -1)
    errExit("initSemAailable");
  if (initSemInUse(semid,READ_SEM) == -1)
    errExit("initSemInUse");

  fd = open(TEMP_FILE,O_CREAT | O_RDWR | O_EXCL,OBJ_PERMS);
  if (fd == -1)
    errExit("open");

  if (ftruncate(fd,sizeof(struct shmseg)) == -1)
    errExit("ftruncate");

  addr = mmap(NULL,sizeof(struct shmseg),PROT_WRITE| PROT_READ,MAP_SHARED,fd,0);
  if (addr == MAP_FAILED)
    errExit("mmap");

  shmp = (struct shmseg *)addr;

  if (close(fd) == -1)
    errExit("close");
  
  for (xfrs = 0,bytes = 0;;xfrs++,bytes += shmp->cnt){
    if (reserveSem(semid,WRITE_SEM) == -1)
      errExit("reserveSem");

    shmp->cnt = read(STDIN_FILENO,shmp->buf,BUF_SIZE);
    if (shmp->cnt == -1)
      errExit("read");

    if (releaseSem(semid,READ_SEM) == -1)
      errExit("reserveSem");

    if (shmp->cnt == 0)
      break;

  }

  if (releaseSem(semid,WRITE_SEM) == -1)
    errExit("ReserveSem");

  if (semctl(semid,0,IPC_RMID,dummy) == -1)
    errExit("semctl");

  if (unlink(TEMP_FILE) == -1)
    errExit("unlink");
  
  fprintf(stderr,"Send %d bytes (%d xfrs)\n",bytes,xfrs);
  exit(EXIT_SUCCESS);
  
   
}
