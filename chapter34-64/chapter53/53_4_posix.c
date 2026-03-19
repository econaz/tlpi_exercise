#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


#define THREAD_NUM 20
#define LOOPS 100000


static sem_t sem;
static volatile long n = 0;


void *threadFunc(void *arg){

  for (int i = 0;i < LOOPS;i++){
    //todo
  }

}
