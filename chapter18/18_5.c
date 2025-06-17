

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 4096

char *mygetcwd(char *buf, size_t size) {

  char path[BUF_SIZE], pname[BUF_SIZE];
  DIR *dirp;
  struct stat st, st_p, st_c;
  struct dirent *dp;
  dev_t dev;
  ino_t ino;
  int fd;

  fd = open(".", O_RDONLY);
  if (fd == -1) {
    printf("error open");
    return NULL;
  }

  sprintf(buf, "");

  while (1) {

    if (stat(".", &st) == -1) {
      printf("error stat");
      return NULL;
    }

    dev = st.st_dev;
    ino = st.st_ino;

    if (stat("..", &st_p) == -1) {
      printf("error stat");
      return NULL;
    }

    if ((dirp = opendir("..")) == NULL) {
      printf("error opendir");
      return NULL;
    }

    if (dev == st_p.st_dev && ino == st_p.st_ino)
      break;

    errno = 0;

    while ((dp = readdir(dirp)) != NULL) {
      snprintf(path, BUF_SIZE, "../%s", dp->d_name);
      if (stat(path, &st_c) == -1) {
        printf("error stat");
        return NULL;
      }
      if (dev == st_c.st_dev && ino == st_c.st_ino) {

        memset(pname, 0, sizeof(pname));
        strcat(pname, "/");
        strcat(pname, dp->d_name);
        strcat(pname, buf);

        strncpy(buf, pname, BUF_SIZE);
        break;
      }
    }
    closedir(dirp);
    chdir("..");
  }
  fchdir(fd);
  close(fd);
  return buf;
}

int main(int argc, char *argv[]) {

  char buf[BUF_SIZE];
  mygetcwd(buf, BUF_SIZE);
  printf("%s\n", buf);
  exit(EXIT_SUCCESS);
}
