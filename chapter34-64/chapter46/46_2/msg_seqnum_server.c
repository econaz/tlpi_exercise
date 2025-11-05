#include "msg_seqnum.h"
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MSG_MODE ((S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP))

int main() {
  int msqid, fd;
  struct RequestMsg req;
  struct ResponseMsg resp;
  int seq_num = 0;

  if ((msqid = msgget(IPC_PRIVATE, MSG_MODE)) == -1)
    errExit("msgget");
  if ((fd = open(SERVER_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1)
    errExit("open");
  if (write(fd, &msqid, sizeof(msqid)) != sizeof(msqid))
    errExit("write");
  if (close(fd) == -1)
    errExit("close");

  for (;;) {
    if (msgrcv(msqid, &req, REQUEST_SIZE, 1, 0) == -1)
      errExit("msgrcv");

    resp.mtype = (long)req.pid;
    resp.seqNum = seq_num;
    // we should send the actual bytes we use in msgsnd
    if (msgsnd(msqid, &resp, RESPONSE_SIZE, 0) == -1)
      errExit("msgsnd");

    seq_num += req.seqLen;
  }

  exit(EXIT_FAILURE);
}
