
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_setenv(const char *name, const char *value, int overwrite);

int main(int argc, char *argv[]) {
  my_setenv("tar", "test", 1);
  my_setenv("tar", "test1", 1);
  char *value = getenv("tar");
  printf("%s\n", value);
  exit(EXIT_SUCCESS);
}

int my_setenv(const char *name, const char *value, int overwrite) {

  char *old_value = getenv(name);
  if (old_value == NULL || overwrite != 0) {
    char *str = (char *)malloc(1024);
    strcat(str, name);
    strcat(str, "=");
    strcat(str, value);
    putenv(str);
    return 0;
  }
  return -1;
}
