#define _POSIX_C_SOURCE 199309
#include "tlpi_hdr.h"
#include <signal.h>
#include <time.h>

#define BUF_SIZE 1024
static void handler(int sig) { return; }

char *currTime(const char *format) {

  static char buf[BUF_SIZE];
  time_t t;
  size_t s;
  struct tm *tm;

  t = time(NULL);
  tm = localtime(&t);
  if (tm == NULL)
    return NULL;

  s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

  return (s == 0) ? NULL : buf;
}

void itimerspecFromStr(char *str, struct itimerspec *tsp) {

  char *dupstr, *cptr, *sptr;

  dupstr = strdup(str);

  cptr = strchr(dupstr, ':');
  if (cptr != NULL)
    *cptr = '\0';

  sptr = strchr(dupstr, '/');
  if (sptr != NULL)
    *sptr = '\0';

  tsp->it_value.tv_sec = atoi(dupstr);
  tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

  if (cptr == NULL) {
    tsp->it_interval.tv_sec = 0;
    tsp->it_interval.tv_nsec = 0;
  } else {
    sptr = strchr(cptr + 1, '/');
    if (sptr != NULL)
      *sptr = '\0';
    tsp->it_interval.tv_sec = atoi(cptr + 1);
    tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
  }
  free(dupstr);
}
int main(int argc, char *argv[]) {
  timer_t t, tidptr;
  struct itimerspec val;
  struct sigaction sa;
  int sig;
  sigset_t sigs;
  siginfo_t siginfo;

  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;

  sigaddset(&sigs, SIGALRM);
  if (sigaction(SIGALRM, &sa, NULL) == -1)
    errExit("sigaction");

  if (timer_create(CLOCK_REALTIME, NULL, &t) == -1)
    errExit("timer_create");

  if (timer_settime(&t, 0, &val, NULL) == -1)
    errExit("timer_settime");

  for (;;) {
    sig = sigwaitinfo(&sigs, &siginfo);
    if (sig == -1)
      errExit("sigwaitinfo");
    tidptr = siginfo.si_value.sival_ptr;

    printf("%s, got signal %d", currTime("%T"), sig);
    printf("sival_ptr  = %ld\n", (long)tidptr);
    printf("time_gotoverun() = %d\n", timer_getoverrun(tidptr));
  }
  return 0;
}
