#include "tlpi_hdr.h"
#include <error_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  pid_t pid;
  int status;
  struct rusage rus;
  long sum;

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    for (int i = 0; i < 300000; i++) {

      sum = i + i;
      sum = sum * 4;
      sum = sum / 2;
    }
    printf("sum = %ld\n", sum);
    sleep(5);
    printf("exit\n");
    _exit(EXIT_SUCCESS);

  default:
    printf("child before\n");
    if (getrusage(RUSAGE_CHILDREN, &rus) == -1)
      errExit("getrusage");
    printf("utime %ld,%ld\n", (long)rus.ru_utime.tv_sec,
           (long)rus.ru_utime.tv_usec);

    sleep(5);
    printf("child process\n");
    if (getrusage(RUSAGE_CHILDREN, &rus) == -1)
      errExit("getrusage");
    printf("utime %ld,%ld\n", (long)rus.ru_utime.tv_sec,
           (long)rus.ru_utime.tv_usec);

    if (wait(&status) == -1)
      errExit("wait");

    sleep(19);
    printf("child exit\n");
    if (getrusage(RUSAGE_CHILDREN, &rus) == -1)
      errExit("getrusage");
    printf("utime %ld,%ld\n", (long)rus.ru_utime.tv_sec,
           (long)rus.ru_utime.tv_usec);
  }
  exit(EXIT_SUCCESS);
}
