
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *handleStr(char *str, char *file) {

  char *token = strtok(str, ":");
  char *path;

  path = (char *)malloc(100 * sizeof(char));
  if (path == NULL)
    return NULL;

  while (token != NULL) {

    strcpy(path, token);
    strcat(path, "/");
    strcat(path, file);
    if (access(path, F_OK) != -1) {
      return path;
    }
    token = strtok(NULL, ":");
  }
  return NULL;
}

int myExeclp(const char *file, const char *arg, ...) {

  va_list args;
  int i = 1;
  char **a;
  char *argVec[10];
  char cp[10], *env, str[10], *path;

  int flag = 0;

  va_start(args, arg);

  while ((argVec[i++] = va_arg(args, char *)) != NULL)
    ;
  va_end(args);

  strcpy(cp, file);
  argVec[0] = strrchr(cp, '/');
  if (argVec[0] != NULL)
    argVec[0]++;
  else {
    argVec[0] = cp;
    flag = 1;
  }
  if (flag == 1) {
    env = getenv("PATH");
    if (env == NULL) {
      strcpy(str, "/usr/bin/");
      strcat(str, argVec[0]);
      if (access(str, F_OK) != -1)
        execve(str, argVec, NULL);
      strcat(str, "/bin/");
      strcat(str, argVec[0]);
      if (access(str, F_OK) != -1)
        execve(str, argVec, NULL);
      return -1;
    } else {

      path = handleStr(env, argVec[0]);
      execve(path, argVec, NULL);
    }
  } else {
    execve(file, argVec, NULL);
  }
  return 0;
}
int main(int argc, char *argv[]) {

  myExeclp("echo", "echo", "222222", (char *)NULL);
  return EXIT_SUCCESS;
}
