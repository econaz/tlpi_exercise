

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int myaccess(const char *name, int type) {

  uid_t uid;
  gid_t gid;
  int re;

  uid = geteuid();
  gid = getegid();

  seteuid(getuid());
  setegid(getgid());
  re = access(name, type);
  seteuid(uid);
  setegid(gid);
  return re;
}

int main(int argc, char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    printf("usage:<filename>\n ");
    exit(EXIT_SUCCESS);
  }
  if (myaccess(argv[1], F_OK) == 0)
    printf("File exists\n");
  else
    printf("File not exist\n");

  if (myaccess(argv[1], R_OK) == 0)
    printf("File can read\n");
  else
    printf("File not read\n");

  if (myaccess(argv[1], W_OK) == 0)
    printf("File can write\n");
  else
    printf("File can not write\n");

  if (myaccess(argv[1], X_OK) == 0)
    printf("File can execute\n");
  else
    printf("File can not execute\n");
  return EXIT_SUCCESS;
}
