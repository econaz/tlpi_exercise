#define _XOPEN_SOURCE 600
#include <dirent.h>
#include <ftw.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int (*func)(const char *pathname, const struct stat *statbuf,
                    int typeflag, struct FTW *ftwbuf);

int handle(const char *dirpath, int flags) {

  struct stat statbuf;
  DIR *dir;
  struct dirent *dirt;
  int typeflag;
  struct FTW ftwbuf;

  if (flags & FTW_PHYS) {
    if (lstat(dirpath, &statbuf) == -1) {
      return -1;
    }
    if (S_ISLNK(statbuf.st_mode))
      typeflag = FTW_NS;

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

  // if (flags & FTW_DEPTH && typeflag == FTW_D){
  //
  // }

  dirt = readdir(dir);
}

int mynftw(const char *dirpath, func func, int nopenfd, int flags) {}
int main(int argc, char *argv[]) {}
