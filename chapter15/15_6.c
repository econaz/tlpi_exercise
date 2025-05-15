

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int chmod_rx(const char *str) {

  struct stat st;
  mode_t mode;

  if (stat(str, &st) == -1)
    return -1;

  mode = st.st_mode;
  mode = mode | S_IRUSR | S_IRGRP | S_IROTH;

  if (S_ISDIR(st.st_mode))
    mode = mode | S_IXUSR | S_IXGRP | S_IXOTH;
  else if (mode & S_IXUSR)
    mode = mode | S_IXUSR | S_IXGRP | S_IXOTH;

  return chmod(str, mode);
}

int main(int argc, char *argv[]) {
  if (argc == -1 || strcmp(argv[1], "--help") == 0) {
    printf("usage");
    exit(EXIT_SUCCESS);
  }
  int i;

  for (i = 1; i < argc; i++) {
    if (chmod_rx(argv[1]) == -1) {
      printf("error chmod_rx");
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
