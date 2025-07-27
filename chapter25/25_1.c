#include "tlpi_hdr.h"
#include <wait.h>

int main(int argc, char *argv[]) {
  int status;
  pid_t pid = fork();

  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    exit(-1);

  default:
    wait(&status);
  }
  if (WIFEXITED(status))
    printf("%d\n", WEXITSTATUS(status));

  exit(EXIT_SUCCESS);
}
// -1 补码 0xFFFF 因为只用后16位的高8位表示状态码所以是0x0000FF00
//  WIFEXITED 为(status &0x7F) == 0  为true
// WEXITSTATUS 为 (status & 0xFF00) >> 8
// 为 0xFF00 & 0xFF00为0xFF00 右移8位为0x00FF;
// 0x000000FF 十进制为255
