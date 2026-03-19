#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "semun.h"
#include "binary_sems.h"
#include "tlpi_hdr.h"


Boolean bsUseSemUndo = FALSE;
Boolean bsRetryOnEintr = TRUE;


int initSemAvailable(int semId, int semNum,int val){

  union semun arg;

  arg.val = val;
  return semctl(semId, semNum, SETVAL,arg);
}

int initSemInUse(int semId, int semNum){
  union semun arg;

  arg.val = 0;
  return semctl(semId, semNum, SETVAL,arg);
}

int reserveSem(int semId, int semNum){

  struct sembuf sops;

  sops.sem_num = semNum;
  sops.sem_op = -1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

  while (semop(semId, &sops, 1) == -1)
    if (errno != EINTR || !bsRetryOnEintr)
      return -1;

  return 0;
  
}

int releaseSem(int semId, int semNum){

  struct sembuf sops;

  sops.sem_num  = semNum;
  sops.sem_op = 1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

  return semop(semId, &sops,1);
}


int main(int argc,char *argv[]){

  int semid, num;
  struct timeval startTime,endTime;
  double timeUse;
  

  if (argc > 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s [times]\n",argv[0]);

  semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
  if (semid == -1)
    errExit("semid");

  
  if (initSemAvailable(semid, 1, 500000) == -1)
    errExit("initSemAvailable");

  num = atoi(argv[1]);

  gettimeofday(&startTime, NULL);
  
  for (;num > 0;num--){
    if (reserveSem(semid, 1) == -1)
      errExit("reserveSem");
  }

  gettimeofday(&endTime, NULL);
  timeUse = (endTime.tv_sec = startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.00;

  printf("cost time = %lf\n",timeUse);

  exit(EXIT_SUCCESS);
  
  
}
