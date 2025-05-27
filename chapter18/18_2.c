
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;

  mkdir("test", S_IRUSR | S_IWUSR | S_IXUSR);
  chdir("test");
  fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  // 第一个参数感觉像是以当前文件为路径
  // 第二个参数是当前的工作目录
  symlink("test/myfile", "../mylink");
  chmod("../mylink", S_IRUSR);
  exit(EXIT_SUCCESS);
}
