#include "tlpi_hdr.h"
#include <pthread.h>
#include <string.h>

static void *threadFunc(void *arg) {

  struct someStruct *pbuf = (struct someStruct *)arg;

  /*do some work with structure pointed to by 'pbuf' */
}

int main(int argc, char *argv[]) {

  struct someStruct buf;

  pthread_create(&thr, NULL, threadFunc, (void *)&buf);

  pthread_exit(NULL);
}

// buf结构体存在于栈上
// 而主线程退出后，栈被销毁，buf结构体不存在了
// 而线程thr依旧在使用buf,会存在未定义的行为
