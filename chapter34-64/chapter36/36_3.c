#include "tlpi_hdr.h"
#include <stdio.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {

  struct rlimit rlm;

  if (getrlimit(RLIMIT_CPU, &rlm) == -1)
    errExit("getrlimit");

  printf("limit CPU time %lld\n", (long long)rlm.rlim_cur);

  if (rlm.rlim_cur == RLIM_INFINITY)
    printf("true\n");
  long sum = 0;
  for (;;) {

    sum += 2;
  }
  exit(EXIT_SUCCESS);
}
