#include <stdio.h>
#define _BSD_SOURCE
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static void displayStatInfo(const struct stat *sb) {

  printf("File type:              ");
  switch (sb->st_mode & S_IFMT) {
  case S_IFREG:
    printf("regular file\n");
    break;
  case S_IFDIR:
    printf("directory\n");
    break;
  case S_IFCHR:
    printf("character device\n");
    break;
  case S_IFBLK:
    printf("block device\n");
    break;
  case S_IFLNK:
    printf("symbolic (soft) link\n");
    break;
  case S_IFIFO:
    printf("FIFO or pipe\n");
    break;
  case S_IFSOCK:
    printf("socket\n");
    break;
  default:
    printf("unkown file type ?\n");
    break;
  }

  printf("Device containing i-node: major = %ld  minor=%ld\n",
         (long)major(sb->st_dev), (long)minor(sb->st_dev));
  printf("I-node number:             %ld\n", (long)sb->st_ino);
  printf("Mode:                      %lo (%s)\n", (unsigned long)sb->st_mode,
         filePermStr(sb->st_mode, 0));

  if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
}
