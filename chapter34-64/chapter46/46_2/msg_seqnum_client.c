#include "msg_seqnum.h"
#include "tlpi_hdr.h"
#include <error_functions.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int msqid, fd;
  struct RequestMsg req;
  struct ResponseMsg resp;

  if ((fd = open(SERVER_FILE, O_RDONLY)) == -1)
    errExit("open");

  if (read(fd, &msqid, sizeof(msqid)) != sizeof(msqid))
    errExit("read");

  if (close(fd) == -1)
    errExit("close");

  req.mtype = 1;
  req.pid = getpid();
  req.seqLen = atoi(argv[1]);

  if (msgsnd(msqid, &req, REQUEST_SIZE, 0) == -1)
    errExit("msgsnd");

  if (msgrcv(msqid, &resp, RESPONSE_SIZE, getpid(), 0) == -1)
    errExit("msgrcv");

  printf("%d\n", resp.seqNum);
}
