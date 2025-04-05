#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct Node {
  char *names;
  pid_t pid;
  int size;
  struct Node *brother;
  struct Node *child;
};
struct status {
  pid_t pid;
  pid_t ppid;
  char *names;
};

struct Node *head = NULL;

void initHead() { head = (struct Node *)malloc(sizeof(struct Node)); }

struct Node *buildNode(char *names, pid_t pid, struct Node *brother,
                       struct Node *child) {

  struct Node *re;
  re = (struct Node *)malloc(sizeof(struct Node));
  re->names = names;
  re->pid = pid;
  re->brother = brother;
  re->child = child;
  return re;
}
void printTree(struct Node *head, int size, char *flag) {

  // 250c |_ 252c --| 2514   251c
  if (head == NULL)
    return;
  while (head) {

    printf("%s", head->names);
    if (head->child) {
      if (head->child->brother) {
        // u252c ┬
        printf("\u2500\u252c\u2500");
        flag[size + 2] = 'a';

      } else
        printf("\u2500\u2500\u2500");

      int sizein = size + strlen(head->names) + 3;
      printTree(head->child, sizein, flag);
    }
    head = head->brother;
    if (head) {
      printf("\n");

      for (int i = 0; i < size - 2; i++) {
        if (flag[i] == 'a')
          printf("\u2502");
        else
          printf(" ");
      }
      if (head->brother)
        // u251c ├
        printf("\u251c\u2500");
      // u2514 └
      else
        printf("\u2514\u2500");
    }
  }
}
// int main(int argc, char *argv[]) {
//   struct Node *second = buildNode("2", 2, NULL, NULL);
//   struct Node *first = buildNode("1", 1, NULL, NULL);
//
//   struct Node *third = buildNode("3", 3, NULL, NULL);
//   struct Node *fourth = buildNode("5", 5, NULL, NULL);
//
//   struct Node *fith = buildNode("4", 4, NULL, NULL);
//
//   struct Node *six = buildNode("6", 6, NULL, NULL);
//   struct Node *seven = buildNode("7", 7, NULL, NULL);
//   struct Node *eight = buildNode("8", 8, NULL, NULL);
//   struct Node *e = buildNode("9", 8, NULL, NULL);
//
//   struct Node *e1 = buildNode("18", 8, NULL, NULL);
//   struct Node *e2 = buildNode("28", 8, NULL, NULL);
//   struct Node *e3 = buildNode("38", 8, NULL, NULL);
//
//   first->child = second;
//   second->child = third;
//   third->brother = fourth;
//   third->child = six;
//   six->child = seven;
//
//   seven->brother = fith;
//   fith->brother = e;
//   // fourth->brother = fith;
//   six->brother = e1;
//   e1->brother = e2;
//   e2->brother = e3;
//
//   // third->child = six;
//   // six->child = second;
//   // six->brother = eight;
//   char *flag = (char *)malloc(sizeof(char));
//   printTree(first, 0, flag);
// }

struct status *getPPid(FILE *fp) {
  char line[64];
  pid_t ppid = -1, pid = -1;
  int i, j, k, l = -1;
  char *names;
  struct status *stus;

  names = (char *)malloc(60 * sizeof(char));

  while (fgets(line, 64, fp) != NULL) {

    if (strncmp(line, "PPid:", 5) == 0) {
      sscanf(line, "PPid:%u", &ppid);
    }
    if (strncmp(line, "Pid:", 4) == 0)
      sscanf(line, "Pid:%u", &pid);
    if (strncmp(line, "Name:", 5) == 0) {
      sscanf(line, "Name:%s", names);
      l = 0;
    }
    if (l == 0 && ppid != -1 && pid != -1) {
      stus = (struct status *)malloc(sizeof(struct status));
      stus->ppid = ppid;
      stus->pid = pid;
      stus->names = names;
      return stus;
    }
  }
  return NULL;
}
void insetNode(struct status *status) {

  if (status->ppid == 0) {
  }
}
char *getcmdline(FILE *fp) {
  char *line;

  line = (char *)malloc(sizeof(char));
  if (fgets(line, 64, fp) != NULL)
    return line;
  return NULL;
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
  struct status *stus;

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
    strtol(dt->d_name, &endptr, 10);

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
      stus = getPPid(fp);
    }
  }
  exit(EXIT_SUCCESS);
}
