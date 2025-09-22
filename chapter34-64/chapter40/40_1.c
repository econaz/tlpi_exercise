#include "tlpi_hdr.h"
#include <error_functions.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>

#define MAX_LINE 32
#define MAX_NAME 32

char *my_getlogin() {

  static char name[MAX_NAME];
  char *devName;
  struct utmpx *ut;

  if ((devName = ttyname(STDIN_FILENO)) == NULL)
    return NULL;

  setutxent();

  printf("%s\n", devName);

  while ((ut = getutxent()) != NULL) {
    if (strncmp(devName + 5, ut->ut_line, MAX_LINE) == 0) {
      strncpy(name, ut->ut_user, MAX_NAME);
      endutxent();
      return name;
    }
  }

  endutxent();
  return NULL;
}

int main(int argc, char *argv[]) {
  char *name;
  if ((name = my_getlogin()) == NULL)
    errExit("my_getlogin");

  printf("%s\n", name);

  exit(EXIT_SUCCESS);
}
