#include "tlpi_hdr.h"
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH_A "/tmp/a"
#define SOCK_PATH_B "/tmp/b"

int main(int argc, char *arg[]) {

  int asfd, bsfd, csfd;
  struct sockaddr_un aaddr, baddr;

  if ((asfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    errExit("socket");
  if ((bsfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    errExit("socket");

  if ((csfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    errExit("socket");

  if (unlink(SOCK_PATH_A) == -1 && errno != ENOENT)
    errExit("unlink");
  if (unlink(SOCK_PATH_B) == -1 && errno != ENOENT)
    errExit("unlink");

  memset(&aaddr, 0, sizeof(struct sockaddr_un));
  aaddr.sun_family = AF_UNIX;
  strncpy(aaddr.sun_path, SOCK_PATH_A, sizeof(aaddr.sun_path) - 1);

  memset(&baddr, 0, sizeof(struct sockaddr_un));
  baddr.sun_family = AF_UNIX;
  strncpy(baddr.sun_path, SOCK_PATH_A, sizeof(baddr.sun_path) - 1);

  if (bind(asfd, (struct sockaddr *)&aaddr, sizeof(struct sockaddr_un)) == -1)
    errExit("bind");
  
  if (bind(bsfd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_un)) == -1)
    errExit("bind");

  
}

// todo
