#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void getPPid(FILE *fp) {

  char line[64];
  pid_t ppid = -1;
  int i, j, k, l = -1;
  char *names;

  names = (char *)malloc(60 * sizeof(char));

  while (fgets(line, 64, fp) != NULL) {

    i = sscanf(line, "PPid:%d", &ppid);

    j = sscanf(line, "Name:%s", names);
    if (j > 0)
      l = 0;
    if ()
  }
}
char *getcmdline(FILE *fp) {
  char *line;

  line = (char *)malloc(sizeof(char));
  if (fgets(line, 64, fp) != NULL)
    return line;
}
int main(int argc, char *argv[]) {
  struct dirent *dt;
  DIR *dirs;
  char *endptr;
  pid_t pid;
  uid_t uid;
  char str[50] = "/proc/";
  FILE *fp;
  char *names;
  // struct NameAndUid nad;

  names = (char *)malloc(40 * (sizeof(char)));
  if (names == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }

  // open /proc/ dir
  dirs = opendir("/proc");
  if (dirs == NULL) {
    printf("dir error\n");
    exit(EXIT_FAILURE);
  }
  while ((dt = readdir(dirs)) != NULL) {
    pid = strtol(dt->d_name, &endptr, 10);

    // chech if dt_type is dir
    if (dt->d_type == DT_DIR && *endptr == '\0') {
      // cat string
      strcat(str, dt->d_name);
      strcat(str, "/status");

      // open file
      fp = fopen(str, "r");
      if (fp == NULL) {
        printf("file open error");
        exit(EXIT_FAILURE);
      }
      // init str
      strcpy(str, "/proc/");
    }
  }
  exit(EXIT_SUCCESS);
}
