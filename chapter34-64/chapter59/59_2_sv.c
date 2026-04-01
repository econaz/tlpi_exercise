#define _BSD_SOURCE

#include "inet_socket.h"
#include "is_seqnum.h"
#include <netdb.h>

#define BACKLOG 50

int main(int argc, char *argv[]) {

  uint32_t seqNum;
  char reqLenStr[INT_LEN];
  char seqNumStr[INT_LEN];
  struct sockaddr_storage claddr;
  int lfd, cfd, reqLen;
  socklen_t addrlen;

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
  char addrStr[ADDRSTRLEN];

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [init-seq-num]\n", argv[0]);

  seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    errExit("signal");

  if ((lfd = inetListen(PORT_NUM, BACKLOG, &addrlen)) == -1)
    errExit("inetListen");

  for (;;) {
    addrlen = sizeof(struct sockaddr_storage);
    cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
    if (cfd == -1) {
      errMsg("accept");
      continue;
    }

    inetAddressStr((struct sockaddr *)&claddr, addrlen, addrStr, ADDRSTRLEN);
    printf("Connection from %s\n", addrStr);
    if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
      close(cfd);
      continue;
    }
    reqLen = atoi(reqLenStr);
    if (reqLen <= 0) {
      close(cfd);
      continue;
    }

    snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
    if (write(cfd, seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
      fprintf(stderr, "Error on write");

    seqNum += reqLen;

    if (close(cfd) == -1)
      errMsg("close");
  }
}
