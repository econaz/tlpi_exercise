

#include "tlpi_hdr.h"
#include <assert.h>

int main(int argc, char *argv[]) {

  pid_t cpid;
  cpid = vfork();
  switch (cpid) {
  case -1:
    errExit("vfork");
  case 0:
    if (close(STDIN_FILENO) == -1)
      errExit("close");
    assert(write(STDIN_FILENO, "pid\n", 2) == -1);
    _exit(EXIT_SUCCESS);
  default:
    if (write(STDIN_FILENO, "ppid\n", 5) != -1)
      errExit("fa write");
    exit(EXIT_SUCCESS);
  }
}
