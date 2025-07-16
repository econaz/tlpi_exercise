#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>

unsigned int alarm(unsigned int seconds) {

  struct itimerval new, old;
  new.it_interval.tv_sec = 0;
  new.it_interval.tv_usec = 0;
  new.it_value.tv_sec = seconds;
  new.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &new, &old);
  return old.it_interval.tv_sec;
}
int main(int argc, char *argv[]) {

  printf("%u\n", alarm(10));
  printf("%u\n", alarm(7));

  for (;;) {
  }
}
