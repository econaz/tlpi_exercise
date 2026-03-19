#include <error_functions.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tlpi_hdr.h"


int main(int argc,char *argv[]){

  sem_t *sem;
  struct timespec tsp;
  
  if (argc < 3 || strcmp(argv[1],"--help") == 0)
    usageErr("%s sem-name\n",argv[0]);

  sem = sem_open(argv[1], 0);
  if (sem == SEM_FAILED)
    errExit("sem_open");

  tsp.tv_sec = atoi(argv[2]);

  if (sem_timedwait(sem, &tsp) == -1)
    errExit("sem_timedwait");

  printf("%ld sem_timedwait() succeedd\n",(long)getpid());

  exit(EXIT_SUCCESS);
}
