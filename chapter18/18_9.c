

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int fchdir_c() {

  int fd_old, fd_new, i = 0;
  char *curpath;
  clock_t start, end;
  float t;

  fd_new = open("~/Downloads", O_RDONLY);
  fd_old = open(".", O_RDONLY);

  start = clock();
  for (i = 0; i < 10000; i++) {
    fchdir(fd_new);
    fchdir(fd_old);
  }
  end = clock();
  t = (float)(end - start) / CLOCKS_PER_SEC;

  close(fd_old);
  close(fd_new);

  printf("fchdir cast time %f\n", t);
  return 0;
}
int chdir_c() {
  int i = 0;
  char *curpath;
  clock_t start, end;
  float t;

  start = clock();
  for (i = 0; i < 10000; i++) {
    chdir("~/Downloads/");
    chdir(".");
  }
  end = clock();
  t = (float)(end - start) / CLOCKS_PER_SEC;

  printf("chdir cost time %f\n", t);
  return 0;
}

int main(int argc, char *argv[]) {
  chdir_c();
  fchdir_c();
}

// fchdir比chdir快
//  盲猜chdir还需要解析字符串，得到inode,
//  而fchdir不需要解析，直接使用fd；
