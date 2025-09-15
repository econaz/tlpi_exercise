#include "tlpi_hdr.h"
#include <stdio.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void rusage(const struct rusage *rus) {

  printf("CPU time user %.2f sys %.2f\n",
         (rus->ru_utime.tv_sec + rus->ru_utime.tv_usec / 1000000.0),
         (rus->ru_stime.tv_usec + rus->ru_stime.tv_sec / 1000000.0));
  printf("Max rss %ld\n", rus->ru_maxrss);
  printf("Soft page faults %ld\n", rus->ru_minflt);
  printf("Hard page faults %ld\n", rus->ru_majflt);
  printf("Block input operations %ld\n", rus->ru_inblock);
  printf("Block output operations %ld\n", rus->ru_oublock);
  printf("Voluntary context switches %ld\n", rus->ru_nvcsw);
  printf("Involuntary context switches %ld\n", rus->ru_nivcsw);
}

int main(int argc, char *argv[]) {
  struct rusage rus;
  pid_t pid;
  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    execvp(argv[1], &argv[1]);
    _exit(EXIT_SUCCESS);
  default:
    if (waitpid(pid, NULL, 0) == -1)
      errExit("waitpid");
    if (getrusage(RUSAGE_CHILDREN, &rus) == -1)
      errExit("getrusage");
    rusage(&rus);
  }
  exit(EXIT_SUCCESS);
}
