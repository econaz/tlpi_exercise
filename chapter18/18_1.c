

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("sss");
  sleep(1000000000);
  exit(EXIT_SUCCESS);
}

// i-node 变了 应该是先unlink原先的文件，创建了一个新的文件
// 知道程序运行结束才删除原先的文件
