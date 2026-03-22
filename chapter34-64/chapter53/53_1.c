#include "tlpi_hdr.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 4096

static char buffer[BUF_SIZE];
static sem_t wSem;
static sem_t rSem;
static volatile ssize_t num = 0;

void *readFunc(void *arg) {

  int xfrs, bytes;

  for (xfrs = 0, bytes = 0;; xfrs++) {

    if (sem_wait(&rSem) == -1)
      errExit("sem_wait");

    if (num == 0)
      break;
    bytes += num;

    if (write(STDOUT_FILENO, buffer, num) != num)
      fatal("partial/failed write");

    if (sem_post(&wSem) == -1)
      errExit("sem_post");
  }

  if (sem_post(&wSem) == -1)
    errExit("sem_wait");

  fprintf(stderr, "Recevie %d bytes (%d xfrs)\n", bytes, xfrs);

  return NULL;
}

void *writeFunc(void *arg) {

  int xfrs, bytes;

  for (xfrs = 0, bytes = 0;; xfrs++, bytes += num) {
    if (sem_wait(&wSem) == -1)
      errExit("sem_wait");

    num = read(STDIN_FILENO, buffer, BUF_SIZE);
    if (num == -1)
      errExit("read");

    if (sem_post(&rSem) == -1)
      errExit("sem_post");

    if (num == 0)
      break;
  }

  fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);

  if (sem_wait(&rSem) == -1)
    errExit("sem_post");

  return NULL;
}

int main(int argc, char *arg[]) {

  pthread_t rThread, wThread;
  int s;

  if (sem_init(&wSem, 0, 1) == -1)
    errExit("sem_init");

  if (sem_init(&rSem, 0, 0) == -1)
    errExit("sem_init");

  s = pthread_create(&wThread, NULL, writeFunc, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");

  s = pthread_create(&rThread, NULL, readFunc, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");

  s = pthread_join(wThread, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");

  s = pthread_join(rThread, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");

  exit(EXIT_SUCCESS);
}
