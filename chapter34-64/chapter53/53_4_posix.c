#include "tlpi_hdr.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

#define THREAD_NUM 20
#define LOOPS 100000

static sem_t sem;

void *threadFunc(void *arg) {

  for (int i = 0; i < LOOPS; i++) {
    // todo
    sem_wait(&sem);
    sem_post(&sem);
  }
  return NULL;
}

int main(int argc, char *argv[]) {

  pthread_t thr[THREAD_NUM];
  int pthreadNums;
  struct timeval startTime, endTime;
  double timeUse;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s usage: thread-nums\n", argv[0]);

  pthreadNums = atoi(argv[1]);

  sem_init(&sem, 0, 1);

  gettimeofday(&startTime, NULL);

  for (int i = 0; i < pthreadNums; i++)
    if (pthread_create(&thr[i], NULL, threadFunc, NULL) != 0)
      errExit("pthread_create");

  for (int i = 0; i < pthreadNums; i++)
    if (pthread_join(thr[i], NULL) != 0)
      errExit("pthread_join");

  gettimeofday(&endTime, NULL);

  timeUse = (endTime.tv_sec - startTime.tv_sec) +
            (endTime.tv_usec - startTime.tv_usec) / 1000000.00;

  printf("use time = %lf\n", timeUse);
}
