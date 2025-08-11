#include "tlpi_hdr.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

struct thread_info {
  pthread_t t;
  int loops;
};

static volatile int glob = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *threadFun(void *arg) {

  struct thread_info *threadInfo = (struct thread_info *)arg;
  int loops = threadInfo->loops;

  int j;

  for (j = 0; j < loops; j++) {
    pthread_mutex_lock(&mutex);
    glob++;
    printf("count = %d,thread_id = %ld\n", glob, threadInfo->t);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  struct thread_info t1, t2;
  int s;

  t1.loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loop") : 10000000;
  t2.loops = t1.loops;

  s = pthread_create(&t1.t, NULL, threadFun, (void *)(&t1));
  if (s != 0)
    errExitEN(s, "pthread_create");

  s = pthread_create(&t2.t, NULL, threadFun, (void *)(&t2));
  if (s != 0)
    errExitEN(s, "pthread_create");

  s = pthread_join(t1.t, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");
  s = pthread_join(t2.t, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");

  printf("glob = %d\n", glob);
  exit(EXIT_SUCCESS);
}
