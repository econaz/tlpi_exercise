
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/stat.h>

key_t my_ftok(const char *pathname, int j) {

  struct stat buf;
  key_t key;

  if (stat(pathname, &buf) == -1)
    return -1;

  return ((j & 0xff) << 24) + ((buf.st_dev & 0xff) << 16) +
         (buf.st_ino & 0xffff);
}

int main(int argc, char *argv[]) {

  key_t key = ftok("45_2.c", 2);
  printf("%x\n", (int)key);
  key_t mykey = my_ftok("45_2.c", 2);
  printf("%x\n", (int)mykey);
}
