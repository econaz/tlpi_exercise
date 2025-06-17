

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define _XOPEN_SOURCE 6O0
#include "tlpi_hdr.h"
#include <ftw.h>

static void usageError(const char *progName, const char *msg) {

  if (msg != NULL)
    fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", progName);
  fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
  fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
  fprintf(stderr, "\t-p Use FTW_PHYS flag\n");
  exit(EXIT_FAILURE);
}
static int dirTree(const char *pathname, const struct stat *sbuf, int type,
                   struct FTW *ftwb) {

  switch (sbuf->st_mode & S_IFMT) {
  case S_IFREG:
    printf("-");
    break;
  case S_IFDIR:
    printf("d");
    break;
  case S_IFCHR:
    printf("c");
    break;
  case S_IFBLK:
    printf("b");
    break;
  case S_IFLNK:
    printf("l");
    break;
  case S_IFIFO:
    printf("p");
    break;
  case S_IFSOCK:
    printf("s");
    break;
  default:
    printf("?");
    break;
  }
  printf("%s   ",
         (type == FTW_D) ? "D   ": (type == FTW_DNR) ? "DNR":
}
