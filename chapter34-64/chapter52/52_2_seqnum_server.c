#include <signal.h>
#include <mqueue.h>
#include "mq_seqnum.h"


int main(int argc,char *argv[])
{
  mqd_t serverMq,clientMq;
  char clientFifo[CLIENT_FIFO_NAME_LEN];
  struct request req;
  struct response resp;
  int seqNum = 0;
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_msgsize = sizeof(req);
  attr.mq_maxmsg = 10;

  serverMq = mq_open(SERVER_FIFO, O_CREAT | O_EXCL | O_RDWR,S_IRUSR | S_IWUSR,&attr);
  if (serverMq == -1)
    errExit("mq_open");

  for(;;){
    if (mq_receive(serverMq,(char *) &req, attr.mq_msgsize, NULL) != sizeof(req)){
      fprintf(stderr, "Error reading request;discarding\n");
      continue;
    }

    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) req.pid);

    if ((clientMq = mq_open(clientFifo, O_RDONLY,0,attr)) == -1){
      errMsg("open %s",clientFifo);
      continue;
    }
    resp.seqNum = seqNum;
    if (mq_send(clientMq, (char *)&resp, attr.mq_msgsize, NULL) == -1)
      fprintf(stderr, "Error writing to FIFO %s\n",clientFifo);
    if (mq_close(clientMq) == -1)
      errMsg("close");

    seqNum += req.seqLen;
  }
}
