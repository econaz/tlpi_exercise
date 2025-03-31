#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

uid_t userIdFromName(const char *name) {
  struct passwd *pwd;
  uid_t u;
  char *endptr;

  if (name == NULL || *name == '\0')
    return -1;

  u = strtol(name, &endptr, 10);
  if (*endptr == '\0')
    return u;

  pwd = getpwnam(name);
  if (pwd == NULL)
    return -1;

  return pwd->pw_uid;
}
char *getNameFromStatus(FILE *fp, uid_t uid) {
  char *names;
  int rid = -1, eid = -1, sid = -1, fid = -1;
  char line[64];
  int i, j, k = -1;

  names = (char *)malloc(40 * sizeof(char));
  if (names == NULL) {
    printf("error malloc\n");
    return NULL;
  }

  while (fgets(line, 64, fp) != NULL) {
    i = sscanf(line, "Name:%s", names);
    if (i > 0) {
      k = 0;
    }
    j = sscanf(line, "Uid:%d%d%d%d", &rid, &eid, &sid, &fid);
    // if rid = uid and find name
    if (j > 0 && rid == uid && k != -1) {
      fclose(fp);
      return names;
    }
  }
  fclose(fp);
  return NULL;
}

int main(int argc, char *argv[]) {
  struct dirent *dt;
  DIR *dirs;
  char *endptr;
  pid_t pid;
  uid_t uid;
  char str[1024] = "/proc/";
  FILE *fp;
  char *names;
  // struct NameAndUid nad;

  names = (char *)malloc(40 * (sizeof(char)));
  if (names == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }

  // find id from name
  uid = userIdFromName("econs");
  if (uid == -1) {
    printf("error userIdFromName\n");
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
      // getname if uid = rid
      names = getNameFromStatus(fp, uid);
      if (names != NULL)
        printf("names = %s,id = %u\n", names, pid);
    }
  }
  exit(EXIT_SUCCESS);
}
