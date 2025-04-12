

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

  int i, rd;

  for (i = 0; i < num; i++) {
    srand((unsigned)time(NULL) + (unsigned)rand());
    rd = rand() % 100000;
    seq[i] = rd;
  }
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

  for (i = 0; i < num; i++) {
    if (seq == NULL) {
      sprintf(filename, "%s/x%06d", dir, i);
    } else {
      sprintf(filename, "%s/x%06d", dir, seq[i]);
    }
    if (remove(filename) == -1) {
      return -1;
    }
  }
  return 0;
}
int main(int argc, char *argv[]) {

  int num;
  char *dir;
  int random = 0;
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

  if (random) {
    int seq[num];
    creatRandom(seq, num);
    creatFileRandom(seq, dir, num);
    // deleteFile(seq, dir, num);
  } else {
    creatFile(dir, num);
    // deleteFile(NULL, dir, num);
  }
  exit(EXIT_SUCCESS);
}
