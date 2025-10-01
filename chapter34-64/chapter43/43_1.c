#include "tlpi_hdr.h"
#include <error_functions.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipeFd[2];
  pid_t pid;
  int dataNumber, dataSize;
  struct timeval start, end;
  void *ptr;
  long second, micosecond;
  double cost, costPer;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("usage: %s dataNumber dataSize\n", argv[0]);

  dataNumber = getInt(argv[1], GN_ANY_BASE, "dataNumber");
  dataSize = getInt(argv[2], GN_ANY_BASE, "dataSize");

  printf("dataNumber %d\n", dataNumber);
  printf("dataSize %d\n", dataSize);

  if (pipe(pipeFd) == -1)
    errExit("pipe");

  ptr = calloc(dataNumber, dataSize);
  if (ptr == NULL)
    errExit("calloc");

  memset(ptr, 'c', dataNumber * dataSize);

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    close(pipeFd[0]);
    if (write(pipeFd[1], ptr, dataNumber * dataSize) != dataSize * dataNumber)
      errExit("write");

    printf("send to parent success\n");

    close(pipeFd[1]);

    exit(EXIT_SUCCESS);

  default:
    break;
  }
  gettimeofday(&start, 0);

  close(pipeFd[1]);
  if (read(pipeFd[0], ptr, dataNumber * dataSize) != dataSize * dataNumber)
    errExit("read");

  printf("read to child success\n");

  close(pipeFd[0]);

  gettimeofday(&end, 0);

  second = end.tv_sec - start.tv_sec;
  micosecond = end.tv_usec - start.tv_usec;

  cost = second + micosecond * 1e-6;

  costPer = (dataSize * dataNumber) / cost;

  printf("cost %f\n", cost);
  printf("recevie data from parent per second :%lf\n", costPer);

  exit(EXIT_SUCCESS);
}
