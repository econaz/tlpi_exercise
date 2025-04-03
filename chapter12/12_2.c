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
void printTree(struct Node *head, int size) {

  if (head == NULL)
    return;
  while (head) {
    printf("%s", head->names);
    if (head->child) {
      if (head->child->brother)
        printf("\u2500\u252c\u2500");
      else
        printf("\u2500\u2500\u2500");
      size = strlen(head->names) + 3 + size;
      // printf("%d\n", size);
      printTree(head->child, size);
    }

    head = head->brother;
    if (head) {
      printf("\n");
      // if (head->brother)
      //   printf("11111");
      for (int i = 0; i < size; i++) {
        printf("\u2500");
      }
    }
  }
}
int main(int argc, char *argv[]) {
  struct Node *second = buildNode("2", 2, NULL, NULL);
  struct Node *first = buildNode("1", 1, NULL, NULL);

  struct Node *third = buildNode("3", 3, NULL, NULL);
  struct Node *fourth = buildNode("5", 5, NULL, NULL);

  struct Node *fith = buildNode("4", 4, NULL, NULL);
  first->child = second;
  second->child = third;
  third->brother = fourth;

  fourth->brother = fith;

  printTree(first, 0);
}

void getPPid(FILE *fp) {
  char line[64];
  pid_t ppid = -1;
  int i, j, k, l = -1;
  char *names;

  names = (char *)malloc(60 * sizeof(char));

  while (fgets(line, 64, fp) != NULL) {

    if (strncmp(line, "PPid:", 5) == 0) {
      sscanf(line, "PPid:%u", &ppid);
    }
    if (strncmp(line, "Name:", 5) == 0) {
      sscanf(line, "Name:%s", names);
      l = 0;
    }
    if (l == 0 && ppid != -1) {
    }
  }
}
char *getcmdline(FILE *fp) {
  char *line;

  line = (char *)malloc(sizeof(char));
  if (fgets(line, 64, fp) != NULL)
    return line;
  return NULL;
}

// int main(int argc, char *argv[]) {
//   struct dirent *dt;
//   DIR *dirs;
//   char *endptr;
//   pid_t pid;
//   uid_t uid;
//   char str[50] = "/proc/";
//   FILE *fp;
//   char *names;
//   // struct NameAndUid nad;
//
//   names = (char *)malloc(40 * (sizeof(char)));
//   if (names == NULL) {
//     printf("malloc error\n");
//     exit(EXIT_FAILURE);
//   }
//
//   // open /proc/ dir
//   dirs = opendir("/proc");
//   if (dirs == NULL) {
//     printf("dir error\n");
//     exit(EXIT_FAILURE);
//   }
//   while ((dt = readdir(dirs)) != NULL) {
//     pid = strtol(dt->d_name, &endptr, 10);
//
//     // chech if dt_type is dir
//     if (dt->d_type == DT_DIR && *endptr == '\0') {
//       // cat string
//       strcat(str, dt->d_name);
//       strcat(str, "/status");
//
//       // open file
//       fp = fopen(str, "r");
//       if (fp == NULL) {
//         printf("file open error");
//         exit(EXIT_FAILURE);
//       }
//       // init str
//       strcpy(str, "/proc/");
//     }
//   }
//   exit(EXIT_SUCCESS);
// }
