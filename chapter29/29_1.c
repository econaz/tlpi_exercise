#include "tlpi_hdr.h"
#include <pthread.h>

static void *threadFunc(void *arg) {
  char *s = (char *)arg;

  printf("%s", s);

  return (void *)strlen(s);
}

int main(int argc, char *argv[]) {

  pthread_t t1;
  void *res;
  int s;

  s = pthread_create(&t1, NULL, threadFunc, (void *)"Hello world\n");
  if (s != 0)
    errExitEN(s, "pthread_create");

  printf("Message from main()\n");
  s = pthread_join(pthread_self(), &res);
  // ERROR [EDEADLK/EDEADLOCK Resource deadlock avoided] pthread_join
  // 会发生死锁
  //  pthread_equal(tid,pthread_self()) 判断当前线程
  //  然后在pthread_join(pthread_self(),NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");

  printf("Thread returned %ld\n", (long)res);

  exit(EXIT_SUCCESS);
}
