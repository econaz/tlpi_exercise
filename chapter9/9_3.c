

#include <bits/pthreadtypes.h>
#define _BSD_SOURCE
#include <ctype.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_READ 100

void myinitgroups(const char *user, gid_t group);

int findgroupid(char *line, const char *name) {

  char *ptr;
  int i = 0, j = 0, k = 0, po = 0;

  // find gid number from line
  for (k = 0; line[k] != '\0'; k++) {
    if (line[k] == ':') {
      po += 1;
      if (po == 2) {
        i = k;
      }
      if (po == 3) {
        j = k;
        if (strstr(&line[k + 1], name) == NULL)
          break;
      }
    }
    // covert gid string to int
    if (i != 0 && j != 0) {
      ptr = &line[i + 1];
      ptr[j] = '\0';
      return atoi(ptr);
    }
  }
  return -1;
}

void myinitgroups(const char *user, gid_t group) {

  FILE *fp;
  char buffer[MAX_READ];
  gid_t groupid, *grouplist;
  int size = 0;
  struct passwd *pwd;
  grouplist = (gid_t *)malloc(20);

  // find gid from /etc/group and add git to grouplist
  fp = fopen("/etc/group", "r");

  if (fp == NULL) {
    printf("fopen");
    exit(EXIT_FAILURE);
  }
  while (fgets(buffer, MAX_READ, fp) != NULL) {
    groupid = findgroupid(buffer, user);
    if (groupid == -1) {
      continue;
    }
    grouplist[size++] = groupid;
  }

  // add group to grouplist
  grouplist[size++] = group;

  // find gid from /etc/passwd and add gid to grouplist
  pwd = getpwnam(user);
  if (pwd == NULL)
    exit(EXIT_FAILURE);

  grouplist[size++] = pwd->pw_gid;

  // setgroups
  setgroups(size, grouplist);
}

int main(int argc, char *argv[]) {
  gid_t grouplist[40];
  myinitgroups("econs", 12345);
  getgroups(getgroups(0, grouplist), grouplist);
  for (int i = 0; i < getgroups(0, grouplist); i++) {
    printf("groupid = %d\n", grouplist[i]);
  }
  return EXIT_SUCCESS;
}
