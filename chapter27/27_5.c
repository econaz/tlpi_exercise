

#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Hello wolrd");

  // execlp 会建立新的工作空间，比如堆栈等，
  // printf是行才刷新缓冲区，
  // 因为没有换行副符，所以不会输出，
  // 会被execlp覆盖。
  execlp("sleep", "sleep", "0", (char *)NULL);
  return EXIT_SUCCESS;
}
