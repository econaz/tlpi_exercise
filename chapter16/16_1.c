#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>

int handle(int argc, char *argv[]) {

  char name[1024];
  char value[1024] = "";
  char path[1024];
  int remove = 0, deref = 1, hasvalue = 0, hasname = 0;

  for (int i = 1; i < argc; i++) {

    if (strcmp("-n", argv[i]) == 0) {
      strcpy(name, argv[i + 1]);
      hasname = 1;
    }
    if (strcmp("-v", argv[i]) == 0) {
      strcpy(value, argv[i + 1]);
      hasvalue = 1;
    }
    if (strcmp("-x", argv[i]) == 0) {
      strcpy(name, argv[i + 1]);
      remove = 1;
    }
    if (strcmp("-h", argv[i]) == 0) {
      deref = 0;
    }
    if (argc - 1 == i) {
      strcpy(path, argv[i]);
    }
  }

  if (deref) {
    if (hasname) {
      return setxattr(path, name, value, strlen(value), 0);
    } else {
      return removexattr(path, name);
    }
  } else {
    if (hasname) {
      return lsetxattr(path, name, value, strlen(value), 0);
    } else {
      return removexattr(path, name);
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {

  if (handle(argc, argv) == -1) {
    printf("error hanele");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
