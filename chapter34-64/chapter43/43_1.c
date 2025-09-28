#include "tlpi_hdr.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipeFd[2];
  pid_t pid;
  int dataNumber, dataSize;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("usage: %s dataNumber dataSize\n", argv[0]);

  dataNumber = getInt(argv[1], GN_ANY_BASE, "dataNumber");
  dataSize = getInt(argv[2], GN_ANY_BASE, "dataSize");

  if (pipe(pipeFd) == -1)
    errExit("pipe");

  pid = fork();
  switch (pid) {
  case -1:
    errExit("fork");
  case 0:
    close(pipeFd[0]);
    if (write(pipeFd[1], const void *buf, size_t n))
  }

  return EXIT_SUCCESS;
}
