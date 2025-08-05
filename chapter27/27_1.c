#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname\n", argv[0]);

  execlp(argv[1], argv[1], "hello world", (char *)NULL);
  errExit("execlp");
}

// path里有俩xyz 一个是./dir1 一个是./dir2
// 但是dir1里没有x权限,所以会报错
