#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  char *argVec[] = {"ab", NULL};
  execve("ab", argVec, NULL);
  exit(EXIT_SUCCESS);
}
