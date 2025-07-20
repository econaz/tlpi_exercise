#include "tlpi_hdr.h"
#include <signal.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 1024
#define TIMER_SIG SIGRTMAX

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
  struct itimerspec ts;
  struct sigevent sev;
  time_t *tidlist;
  int sig;
  siginfo_t si;
  sigset_t sigs;
  int j;

  if (argc > 2)
    usageErr("%s secs[/nsec][:int-secs[/int-nsec]]...\n", argv[0]);

  tidlist = calloc(argc - 1, sizeof(time_t));
  if (tidlist == NULL)
    errExit("calloc");

  sigemptyset(&sigs);
  sigaddset(&sigs, TIMER_SIG);
  if (sigprocmask(SIG_SETMASK, &sigs, NULL) == -1)
    errExit("sigprocmask");

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = TIMER_SIG;

  for (j = 0; j < argc - 1; j++) {

    itimerspecFromStr(argv[j + 1], &ts);

    sev.sigev_value.sival_ptr = &tidlist[j];

    if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)
      errExit("timer_create");
    printf("Timer ID :%ld (%s)\n", (long)tidlist[j], argv[j + 1]);

    if (timer_settime(&tidlist[j], 0, &ts, NULL) == -1)
      errExit("timer_settime");
  }

  for (;;) {
    sig = sigwaitinfo(&sigs, &si);
    if (sig == -1)
      errExit("sigwaitinfo");
    time_t *tidptr = si.si_value.sival_ptr;

    printf("[%s] Got signal %d\n", currTime("%T"), sig);
    printf("    *sival_ptr         = %ld\n", (long)*tidptr);
    printf("    timer_getoverrun() = %d\n", timer_getoverrun(tidptr));
  }
}
