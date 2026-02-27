#include "svshm_xfr.h"
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc,char *argv[])
{
  int semid,xfrs,bytes;
  struct shmseg *shmp;
  char *addr;
  int fd;
  

  semid = semget(SEM_KEY,0,0);
  if (semid == -1)
    errExit("semget");

  fd = open(TEMP_FILE,O_RDWR);
  if (fd == -1)
    errExit("open");

  addr = mmap(NULL,sizeof(struct shmseg),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
  if (addr == MAP_FAILED)
    errExit("mmap");

  shmp = (struct shmseg*) addr;

  if(close(fd) == -1)
    errExit("close");

  for (xfrs = 0,bytes = 0;;xfrs++){
    if (reserveSem(semid,READ_SEM) == -1)
      errExit("reserveSem");

    if (shmp->cnt == 0)
      break;
    bytes += shmp->cnt;

    if (write(STDOUT_FILENO,shmp->buf,shmp->cnt) != shmp->cnt)
      fatal("partial/failed write");

    if (releaseSem(semid,WRITE_SEM) == -1)
      errExit("releaseSem");
  }

  if (releaseSem(semid,WRITE_SEM) == -1)
    errExit("releaseSem");

  fprintf(stderr,"Receive %d bytes (%d xfrs)\n",bytes,xfrs);

  exit(EXIT_SUCCESS);
}
