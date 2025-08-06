
#include "tlpi_hdr.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int myExeclp(const char *file, const char *arg, ...) {

  va_list args;
  va_start(args, arg);
  char *arg1[10];
  int i = 0;
  char **a;

  while ((arg1[i++] = va_arg(args, char *)) != NULL)
    ;

  for (a = arg1; *a != NULL; a++) {
    printf("%s\n", *a);
  }

  return 0;
  // execve();
}
int main(int argc, char *argv[]) {

  myExeclp("test1", "test2", "222221", "1111", (char *)NULL);
  return EXIT_SUCCESS;
}
