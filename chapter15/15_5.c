

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  mode_t old = umask(0);
  umask(old);
  exit(EXIT_SUCCESS);
}
