

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }
int creatFile(char *dir, int num) {

  char filename[100];
  FILE *fp;
  int i;
  for (i = 0; i < num; i++) {

    sprintf(filename, "%s/x%06d", dir, i);
    printf("%s\n", filename);
    fp = fopen(filename, "w+");
    if (fp == NULL) {
      printf("fopen error");
      return -1;
    }
    fputc('c', fp);
  }
  return 0;
}

void creatRandom(int seq[], int num) {

  int i, rd, k;
  printf("%d\n", seq[100]);
  for (i = 0; i < num; i++) {
    srand((unsigned)time(NULL) + (unsigned)rand());
    rd = rand() % 100000;
    printf("%d\n", rd);

    for (k = i - 1; k >= 0; k--) {
      if (seq[k] == seq[i])
        break;
    }
    if (seq[k] == seq[i])
      i--;
    else
      seq[i] = rd;
  }
}
int checkDir(char *dir) {

  if (access(dir, F_OK) != 0) {
    if (mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
      return -1;
  }
  return 0;
}

int creatFileRandom(int seq[], char *dir, int num) {
  char filename[20];
  FILE *fp;
  int i;

  for (i = 0; i < num; i++) {
    // printf("%d\n", rd);
    sprintf(filename, "%s/x%06d", dir, seq[i]);
    printf("%s\n", filename);
    fp = fopen(filename, "w+");
    if (fp == NULL) {
      printf("fopen error");
      return -1;
    }
    fputc('c', fp);
  }
  return 0;
}
int deleteFile(int seq[], char *dir, int num) {

  FILE *fp;
  char filename[20];
  int i;
  qsort(seq, num, sizeof(int), compare);

  // for (int k = 0; k < num; k++) {
  //
  //   printf("%d\n", seq[k]);
  // }
  for (i = 0; i < num; i++) {
    if (seq == NULL) {
      sprintf(filename, "%s/x%06d", dir, i);
    } else {
      sprintf(filename, "%s/x%06d", dir, seq[i]);
    }
    if (remove(filename) == -1) {
      printf("\n");
      printf("%s\n", filename);
      printf("remove file error");
      return -1;
    } else {
      printf("%s success!\n", filename);
    }
  }
  return 0;
}
int main(int argc, char *argv[]) {

  int num;
  char *dir;
  int random = 0;
  int *seq = NULL;
  if (argc < 3) {
    printf("argc error");
    exit(EXIT_FAILURE);
  }
  if (argc == 4 && strcmp(argv[3], "-r") == 0) {
    random = 1;
  }
  dir = argv[1];
  num = atoi(argv[2]);
  if (num == -1) {
    printf("atoi error");
    exit(EXIT_FAILURE);
  }

  if (checkDir(dir) == -1) {
    printf("checkDir error!");
    exit(EXIT_FAILURE);
  }
  if (random) {
    printf("%d\n", num);
    seq = (int *)malloc(sizeof(int) * (num));
    creatRandom(seq, num);
    // creatFileRandom(seq, dir, num);
    // deleteFile(seq, dir, num);
  } else {
    creatFile(dir, num);
    deleteFile(NULL, dir, num);
  }
  exit(EXIT_SUCCESS);
}
