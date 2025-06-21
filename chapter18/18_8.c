#define _XOPEN_SOURCE 600
#include <dirent.h>
#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int (*func)(const char *pathname, const struct stat *statbuf,
                    int typeflag, struct FTW *ftwbuf);

int handle(const char *dirpath, int flags, func fun, int level) {
  struct stat statbuf;
  DIR *dir;
  struct dirent *dirt;
  int typeflag;
  struct FTW ftwbuf;

  if (flags & FTW_PHYS) {
    if (lstat(dirpath, &statbuf) == -1)
      return -1;
    if (S_ISLNK(statbuf.st_mode))
      typeflag = FTW_SL;
  } else {
    if (stat(dirpath, &statbuf) == -1)
      return -1;
  }
  if (flags & FTW_CHDIR) {
    if (chdir(dirpath) == -1)
      return -1;
  }

  if (S_ISDIR(statbuf.st_mode)) {
    typeflag = FTW_D;
    if (flags & FTW_DEPTH) {
      typeflag = FTW_DP;
    }
  }
  if (access(dirpath, R_OK) == -1) {
    typeflag = FTW_DNR;
  }
  if (!S_ISDIR(statbuf.st_mode) && !S_ISLNK(statbuf.st_mode))
    typeflag = FTW_F;

  ftwbuf.base = strlen(dirpath);
  ftwbuf.level = level;
  fun(dirpath, &statbuf, typeflag, &ftwbuf);
  return 1;
}

int ftwc(const char *dirpath, func func, int nopenfd, int flags, int level) {
  DIR *dirp;
  struct dirent *dirt;
  struct stat statbuf;
  char pathname[4096];

  if (flags & FTW_PHYS)
    lstat(dirpath, &statbuf);
  else
    stat(dirpath, &statbuf);

  if (!(flags & FTW_DEPTH)) {
    handle(dirpath, flags, func, level);
  }
  if (S_ISDIR(statbuf.st_mode)) {
    dirp = opendir(dirpath);
    if (dirp == NULL) {
      return -1;
    }
    while ((dirt = readdir(dirp)) != NULL) {
      level += 1;
      if (strcmp(dirt->d_name, "..") == 0 || strcmp(dirt->d_name, ".") == 0)
        continue;
      strncpy(pathname, dirpath, 4096);
      strcat(pathname, "/");
      strcat(pathname, dirt->d_name);
      ftwc(pathname, func, nopenfd, flags, level);
    }
    if (flags & FTW_DEPTH) {
      handle(dirpath, flags, func, level);
    }
    closedir(dirp);
  }
  return 1;
}

int mynftw(const char *dirpath, func func, int nopenfd, int flags) {
  ftwc(dirpath, func, nopenfd, flags, 0);
  return 1;
}
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
  // flag |= FTW_DEPTH;
  if (mynftw(argv[1], calfile, 10, flag) == -1) {
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
