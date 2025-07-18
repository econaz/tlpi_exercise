#include <bits/time.h>
#include <cerrno>
#include <cstdlib>
#include <error_functions.h>
#include <stdio.h>
#include <string.h>
#define _POSIX_C_SOURCE 199309
#include "tlpi_hdr.h"
#include <signal.h>
#include <sys/time.h>
#include <time.h>

static void sigintHandler(int sig) { return; }

int main(int argc, char *argv[]) {

  struct timeval start, finish;
  struct timespec request, cur;
  struct sigaction sa;
  int s;
  long sec, nsec;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s secs nanosecs\n", argv[0]);

  if (clock_gettime(CLOCK_REALTIME, &request) == -1)
    errExit("clock_gettime");

  request.tv_sec += getLong(argv[1], 0, "secs");
  request.tv_nsec += getLong(argv[2], 0, "nanosecs");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigintHandler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    errExit("sigaction");

  if (gettimeofday(&start, NULL) == -1)
    errExit("gettimeofday");

  for (;;) {

    s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);
    // s = nanosleep(&request, &remain);
    if (s == -1 && errno != EINTR)
      errExit("clock_nanosleep");

    if (gettimeofday(&finish, NULL) == -1)
      errExit("gettimeofday");

    printf("Slept for : %9.6f secs\n",
           finish.tv_sec - start.tv_sec +
               (finish.tv_usec - start.tv_usec) / 1000000.0);

    if (s == 0)
      break;

    s = clock_gettime(CLOCK_REALTIME, &cur);
    if (s == -1 && errno != EINTR)
      errExit("clock_gettime\n");

    sec = request.tv_sec - cur.tv_sec;
    nsec = request.tv_nsec - cur.tv_nsec;
    if (nsec < 0) {
      sec -= 1;
      nsec += 1000000000;
    }

    printf("Remaining: %2ld.%09ld\n", sec, nsec);

    // request = remain;
  }
  printf("Sleep complete\n");
  exit(EXIT_SUCCESS);
}
