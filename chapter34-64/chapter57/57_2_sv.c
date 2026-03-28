#include "us_xfr.h"


#define BACKLOG 5


int main(int argc,char *argv[]){

  struct sockaddr_un addr;
  int sfd,cfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1)
    errExit("socket");

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;

  strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    errExit("bind");

  if (listen(sfd, BACKLOG) == -1)
    errExit("listen");

  for (;;){

    cfd = accept(sfd, NULL,NULL);
    if (cfd == -1)
      errExit("accept");

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        fatal("partial/failed write");

    if (numRead == -1)
      errExit("read");
    if (close(cfd) == -1)
      errMsg("close");
    
  }
}
