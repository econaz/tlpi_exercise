#define _GNU_SOURCE
#include "tlpi_hdr.h"
#include <time.h>
#include <utmpx.h>

void my_login(const struct utmpx *ut) {

  setutxent();
  pututxline(ut);
  updwtmpx(_PATH_WTMPX, ut);
}

int my_logout(const char *ut_line) {

  struct utmpx ut;
  ut.ut_type = DEAD_PROCESS;
  time((time_t *)&ut.ut_tv.tv_sec);
  strncpy(ut.ut_line, ut_line, 32);
  memset(&ut.ut_user, 0, sizeof(ut.ut_user));

  setutxent();
  if (pututxline(&ut) == NULL)
    return -1;
  updwtmpx(_PATH_WTMP, &ut);
  endutxent();
  return 0;
}

void my_loginwtmp(const char *line, const char *name, const char *host) {

  struct utmpx ut;

  strncpy(ut.ut_line, line, 32);
  strncpy(ut.ut_user, name, 32);
  strncpy(ut.ut_host, host, 256);

  time((time_t *)&ut.ut_tv.tv_sec);
  ut.ut_pid = getpid();
  ut.ut_type = strlen(name) == 0 ? DEAD_PROCESS : USER_PROCESS;

  updwtmpx(_PATH_WTMP, &ut);
}
