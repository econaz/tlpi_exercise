#include <sys/time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include "semun.h"
#include "binary_sems.h"
#include "tlpi_hdr.h"

#define LOOPS 100000
#define PTHREAD_NUM 20

Boolean bsUseSemUndo = FALSE;
Boolean bsRetryOnEintr = TRUE;

static volatile int semid;


int initSemAvailable(int semId, int semNum){
  union semun arg;

  arg.val = 1;
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

void *threadFunc(void *arg){

  for (int i =0;i<LOOPS;i++){

    if (reserveSem(semid, 0) == -1)
      errExit("reserveSem");
    if (releaseSem(semid, 0) == -1)
      errExit("releaseSem");
  }
  return NULL;

}

int main(int argc,char *argv[]){

  int num;
  struct timeval startTime,endTime;
  double timeUse;
  pthread_t trs[PTHREAD_NUM];
  

  if (argc > 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s\n",argv[0]);

  semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP);
  if (semid == -1)
    errExit("semid");

  
  if (initSemAvailable(semid, 0) == -1)
    errExit("initSemAvailable");


  gettimeofday(&startTime, NULL);

  for (int i =0;i < PTHREAD_NUM;i++)
    if (pthread_create(&trs[i], NULL, threadFunc, NULL) == -1)
      errExit("pthread_create");

  for (int i = 0;i<PTHREAD_NUM;i++)
    if (pthread_join(trs[i], NULL) == -1)
        errExit("pthread_join");


  gettimeofday(&endTime, NULL);
  timeUse = (endTime.tv_sec = startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.00;

  printf("cost time = %lf\n",timeUse);

  exit(EXIT_SUCCESS);
  
  
}
