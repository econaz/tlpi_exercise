
#define _XOPEN_SOURCE 600
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static int file_c = 0;
static int dir_c = 0;
static int link_c = 0;

int calfile(const char *path, const struct stat *statbuf, int type,
            struct FTW *ftwbuf) {

  if (S_ISREG(statbuf->st_mode))
    file_c++;
  if (S_ISDIR(statbuf->st_mode))
    dir_c++;
  if (S_ISLNK(statbuf->st_mode))
    link_c++;
  return 0;
}

int main(int argc, char *argv[]) {
  int flag = 0;

  flag |= FTW_PHYS;
  if (nftw(argv[1], calfile, 10, flag) == -1) {
    printf("error nftw");
    exit(EXIT_FAILURE);
  }

  printf("percent of reg is %f\n",
         (double)file_c / (file_c + dir_c + link_c) * 100);

  printf("percent of dir is %f\n",
         (double)dir_c / (file_c + dir_c + link_c) * 100);
  printf("percent of link is %f\n",
         (double)link_c / (file_c + dir_c + link_c) * 100);

  exit(EXIT_SUCCESS);
}
