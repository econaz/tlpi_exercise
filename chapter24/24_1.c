#include <unistd.h>

int main(int argc, char *argv[]) {

  fork();
  fork();
  fork();
  return 0;
  //  fork();1
  //  pro1  pro2  第一次fork;
  //  fork();2
  //  pro1 -> pro3  pro2 -> pro4 第二次fork();
  //  fork();3
  //  pro1 -> pro5 pro3->pro6 pro2 -> pro7 pro4 -> pro8;
  //  一共8个进程
}
