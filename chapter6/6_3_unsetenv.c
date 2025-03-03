#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int my_unsetenv(const char *name);
int main(int argc, char *argv[]) {
  clearenv();
  char **p;
  putenv("test=111");
  putenv("test1=222");
  putenv("test2=222");
  putenv("test=111111");
  my_unsetenv("test");
  for (p = environ; *p != NULL; p++) {
    puts(*p);
  }
  exit(EXIT_SUCCESS);
}

int my_unsetenv(const char *name) {

  char *value = getenv(name);
  if (value == NULL)
    return -1;
  char **p;
  char **new_en = (char **)malloc(1024);
  char *str = (char *)malloc(1024);
  int i = 0;
  strcat(str, name);
  strcat(str, "=");
  strcat(str, value);
  for (p = environ; *p != NULL; p++) {
    if (strcmp(str, *p) != 0) {
      new_en[i++] = *p;
    }
  }
  free(environ);
  environ = new_en;
  return 0;
}
