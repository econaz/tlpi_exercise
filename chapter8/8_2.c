#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passwd *my_getpwnam(const char *name);

int main(int argc, char *argv[]) {

  struct passwd *pwd;
  pwd = my_getpwnam("root");

  printf("%ld\n", (long)pwd->pw_uid);
  exit(EXIT_SUCCESS);
}

struct passwd *my_getpwnam(const char *name) {

  struct passwd *pwd;
  while ((pwd = getpwent()) != NULL) {
    if (strcmp(pwd->pw_name, name) == 1)
      return pwd;
  }
  setpwent();
  endpwent();
  return NULL;
}
