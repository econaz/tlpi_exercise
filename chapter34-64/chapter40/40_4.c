
#include "tlpi_hdr.h"
#include <paths.h>
#include <time.h>
#include <utmp.h>
#include <utmpx.h>

void my_who() {

  struct utmpx *ut;

  setutxent();

  while ((ut = getutxent()) != NULL) {
    if (ut->ut_type == INIT_PROCESS || ut->ut_type == LOGIN_PROCESS ||
        ut->ut_type == USER_PROCESS) {
      printf("%s  %s\n", ut->ut_user, ut->ut_line);
      printf("%s", ctime((time_t *)&ut->ut_tv.tv_sec));
    }
  }

  endutxent();
}
