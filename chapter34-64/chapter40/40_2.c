#define _GNU_SOURCE
#include "../../../tlpi_code/ugid_functions.h"
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <lastlog.h>
#include <paths.h>
#include <time.h>
#include <utmp.h>
#include <utmpx.h>

int update_lastlog(struct utmpx *ut) {

  struct lastlog llog;
  int fd, j;
  uid_t uid;
  char username[32];

  fd = open(_PATH_LASTLOG, O_RDONLY);
  if (fd == -1)
    return -1;

  strncpy(username, ut->ut_user, 32);

  uid = userIdFromName(username);

  if (lseek(fd, uid * sizeof(struct lastlog), SEEK_SET) == -1)
    return -1;

  strncpy(llog.ll_host, ut->ut_host, 256);
  strncpy(llog.ll_line, ut->ut_line, 32);

  llog.ll_time = ut->ut_tv.tv_sec;

  if (write(fd, &llog, sizeof(struct lastlog)) != sizeof(struct lastlog))
    return -1;

  close(fd);

  return 0;
}

int update_utmp_wtmp_lastlog(const char *username) {
  struct utmpx ut;
  char *devName;

  ut.ut_type = USER_PROCESS;
  strncpy(ut.ut_user, username, sizeof(ut.ut_user));
  if (time((time_t *)&ut.ut_tv.tv_sec) == -1)
    return -1;
  ut.ut_pid = getpid();

  devName = ttyname(STDIN_FILENO);
  if (devName == NULL)
    return -1;

  if (strlen(devName) <= 8)
    return -1;

  strncpy(ut.ut_line, devName + 5, sizeof(ut.ut_line));
  strncpy(ut.ut_id, devName + 8, sizeof(ut.ut_id));

  printf("Creating login entries in utmp and wtmp\n");
  printf("      using pid %ld,line %.*s,id %.*s\n", (long)ut.ut_pid,
         (int)sizeof(ut.ut_line), ut.ut_line, (int)sizeof(ut.ut_id), ut.ut_id);
  setutxent();
  if (pututxline(&ut) == NULL)
    return -1;

  updwtmpx(_PATH_WTMP, &ut);

  sleep(3);

  ut.ut_type = DEAD_PROCESS;
  time((time_t *)&ut.ut_tv.tv_sec);
  memset(&ut.ut_user, 0, sizeof(ut.ut_user));

  printf("Creating Logout entries in utmp and wtmp\n");
  setutxent();

  if (pututxline(&ut) == NULL)
    return -1;

  updwtmpx(_PATH_WTMP, &ut);

  endutxent();
  return 0;
}

int main(int argc, char *argv[]) {

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [username...]\n", argv[0]);

  if (update_utmp_wtmp_lastlog(argv[1]) == -1)
    errExit("update_utmp_wtmp_lastlog");

  exit(EXIT_SUCCESS);
}
