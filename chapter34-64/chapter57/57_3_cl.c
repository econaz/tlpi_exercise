#include "57_3.h"


int main(int argc,char *argv[]){

  int sfd;
  struct sockaddr_un addr;
  struct request req;
  struct response resp;


  req.seqLen = getInt(argv[1], GN_GT_0, "seqLen");

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(&addr.sun_path[0], SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    errExit("connect");

  if (send(sfd, &req, sizeof(struct request), 0) != sizeof(struct request))
    errExit("send");

  if (recv(sfd, &resp, sizeof(struct response), 0) != sizeof(struct response))
    errExit("recv");

  printf("%d\n",resp.seqNum);

  exit(EXIT_SUCCESS);

}
