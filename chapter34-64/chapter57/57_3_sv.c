#include "57_3.h"



int main(int argc, char *argv[]){

  struct sockaddr_un addr;
  int sfd,cfd;
  struct request req;
  struct response resp;
  int seq = 0;

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1)
    errExit("socket");

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(&addr.sun_path[0], SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    errExit("bind");

  if (listen(sfd, 5) == -1)
    errExit("listen");

  for (;;){

    cfd = accept(sfd,NULL,NULL);
    if (cfd == -1)
      errExit("accept");

    if (recv(cfd, &req, sizeof(struct request), 0) ==-1)
      errExit("recv");

    resp.seqNum = seq;

    if (send(cfd, &resp, sizeof(struct response), 0) == -1)
      errExit("send");

    seq += req.seqLen;
  }
  exit(EXIT_SUCCESS);
}
