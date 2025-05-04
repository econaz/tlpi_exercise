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

  printf("File size:                %lld bytes\n", (long long)sb->st_size);
  printf("Optimal I/O block size :  %ld bytes\n", (long)sb->st_blksize);
  printf("512B block allocated:     %lld\n", (long long)sb->st_blocks);
  printf("Last file access:         %lf\n",
         (double)(sb->st_atim.tv_sec + sb->st_atim.tv_nsec));
  printf("Last file modification:   %lf\n",
         (double)(sb->st_mtim.tv_sec + sb->st_mtim.tv_nsec));
  printf("Last status change:       %lf\n",
         (double)(sb->st_ctim.tv_sec + sb->st_ctim.tv_nsec));
}

int main(int argc, char *argv[]) {
  struct stat sb;
  Boolean statLink;
  int fname;

  statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;

  fname = statLink ? 2 : 1;

  if (fname >= argc || (argc > 1) && strcmp(argv[1], "--help") == 0)
    usageErr("%s [-l] file\n"
             "    -l = use lstat() instead of stat()\n",
             argv[0]);
  if (statLink) {
    if (lstat(argv[fname], &sb) == -1)
      errExit("lstat");
  } else {
    if (stat(argv[fname], &sb) == -1)
      errExit("stat");
  }

  displayStatInfo(&sb);

  exit(EXIT_SUCCESS);
}
