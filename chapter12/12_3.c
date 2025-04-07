#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char *getNames(FILE *fp) {

  char line[64];
  char *names;
  int l = -1;

  names = (char *)malloc(64 * sizeof(char));
  if (names == NULL) {
    printf("getNames malloc error\n");
    return NULL;
  }

  while (fgets(line, 64, fp) != NULL) {

    if (strncmp(line, "Name:", 5) == 0) {
      l = sscanf(line, "Name:%s", names);
      // printf("%s\n", names);
      return l > 0 ? names : NULL;
    }
  }
  return NULL;
}
char *readStatus(char *statusPath) {

  FILE *fp;
  char *names;
  strcat(statusPath, "/status");
  fp = fopen(statusPath, "r");

  if (fp == NULL) {
    printf("fopen error\n");
    return NULL;
  }
  return getNames(fp);
}
void readDirs(char *path) {
  FILE *fp;
  struct dirent *dt, *dt1;
  DIR *dir, *dir1;
  char *endptr;
  char str[64] = "/proc/";
  char strcp[64], strstatus[64];
  char buf[2048];
  ssize_t read;
  char *names;

  dir = opendir("/proc");

  if (dir == NULL) {
    printf("main dir error\n");
    exit(EXIT_FAILURE);
  }

  while ((dt = readdir(dir)) != NULL) {

    strtol(dt->d_name, &endptr, 10);

    if (dt->d_type == DT_DIR && *endptr == '\0') {
      strcat(str, dt->d_name);
      strcpy(strstatus, str);
      strcat(str, "/fd/");

      dir1 = opendir(str);

      while ((dt = readdir(dir1)) != NULL) {
        strcpy(strcp, str);
        if (dt->d_type == DT_LNK) {
          strcat(str, dt->d_name);
          read = readlink(str, buf, 2048);
          if (strncmp(buf, path, strlen(path)) == 0) {
            names = readStatus(strstatus);
            if (names != NULL) {
              printf("%s ---- %s -> %s\n", names, str, path);
            }
          }
        }
        strcpy(str, strcp);
      }
      strcpy(str, "/proc/");
    }
  }
}
int main(int argc, char *argv[]) {
  readDirs("/home/econs/.cache/nvidia/GLCache/a084c958fc5501ddb9b6f6d5d507b3ef/"
           "863b3b572e8373a5/559ef6924a35a0e8.toc");
  exit(EXIT_SUCCESS);
}
