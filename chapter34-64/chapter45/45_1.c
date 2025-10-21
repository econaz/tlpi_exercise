#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

  key_t key;
  struct stat st;

  key = ftok("45_1.c", 4);
  stat("45_1.c", &st);

  printf("key = %x\n", (int)key);
  printf("%x\n", 4 & 0xff);
  printf("%x\n", (int)st.st_dev & 0xff);
  printf("%x\n", (int)st.st_ino & 0xffff);

  exit(EXIT_SUCCESS);
}
