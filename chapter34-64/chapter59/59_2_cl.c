#include "inet_socket.h"
#include "is_seqnum.h"
#include <netdb.h>

int main(int argc, char *argv[]) {
  char *reqLenStr;
  char seqNumStr[INT_LEN];
  int cfd;
  ssize_t numRead;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s server-host [sequence-len]\n", argv[0]);

  if ((cfd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM)) == -1)
    errExit("inetConnect");

  reqLenStr = (argc > 2) ? argv[2] : "1";
  if (write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
    fatal("Partial/failed write (reqLenStr)");
  if (write(cfd, "\n", 1) != 1)
    fatal("Partial/failed write (newline)");

  numRead = readLine(cfd, seqNumStr, INT_LEN);
  if (numRead == -1)
    errExit("readLine");
  if (numRead == 0)
    fatal("Unexpected EOF from server");

  printf("Sequence number: %s", seqNumStr);

  exit(EXIT_SUCCESS);
}
