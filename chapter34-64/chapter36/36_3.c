#include "tlpi_hdr.h"
#include <error_functions.h>
#include <stdio.h>
#include <sys/resource.h>

int printRlimit(const char *msg, int resource) {

  struct rlimit rlim;

  if (getrlimit(resource, &rlim) == -1)
    return -1;

  printf("%s soft=", msg);
  if (rlim.rlim_cur == RLIM_INFINITY)
    printf("infinite");
#ifdef RLIM_SAVED_CUR
  else if (rlim.rlim_cur == RLIM_SAVED_CUR)
    printf("unrepresentable");
#endif
  else
    printf("%lld", (long long)rlim.rlim_cur);

  printf("; hard=");
  if (rlim.rlim_max == RLIM_INFINITY)
    printf("infinite\n");
#ifdef RLIM_SAVED_MAX
  else if (rlim.rlim_max == RLIM_SAVED_MAX)
    printf("unrepresentable");
#endif

  else
    printf("%lld\n", (long long)rlim.rlim_max);
  return 0;
}

int main(int argc, char *argv[]) {

  struct rlimit rlit;

  printRlimit("init CPU TIME", RLIMIT_CPU);

  rlit.rlim_cur = 5;
  rlit.rlim_max = 5;
  if (setrlimit(RLIMIT_CPU, &rlit) == -1)
    errExit("setrlimit");

  long sum = 0;
  for (;;) {

    sum += 2;
  }
  exit(EXIT_SUCCESS);
}
