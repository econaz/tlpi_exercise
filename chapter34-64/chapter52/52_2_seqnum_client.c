#include "mq_seqnum.h"
#include "mqueue.h"


static char clientFifo[CLIENT_FIFO_NAME_LEN];


static void removeFifo(void)
{
  mq_unlink(clientFifo);

}

int main(int argc,char *argv[])
{
  mqd_t serverMq,clientMq;
  struct request req;
  struct response resp;
  struct mq_attr attr;

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [seq-len...]\n",argv[0]);

  snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long)getpid());

  attr.mq_msgsize = sizeof(struct response);
  attr.mq_maxmsg = 10;
  attr.mq_flags = 0;

  if ((clientMq = mq_open(clientFifo, O_CREAT | O_EXCL,S_IRUSR | S_IWUSR,&attr)) == -1)
    errExit("mkfifo %s",clientFifo);

  if (atexit(removeFifo) != 0)
    errExit("atexit");

  req.pid = getpid();
  req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

  if ((serverMq = mq_open(SERVER_FIFO, O_WRONLY)) == -1)
    errExit("open %s",SERVER_FIFO);

  if (mq_send(serverMq, (char *)&req, attr.mq_msgsize, NULL) != sizeof(struct request))
    fatal("Can't send to server");

  clientMq = mq_open(clientFifo, O_RDONLY);
  if (clientMq == -1)
    errExit("mq_open");

  if (mq_receive(clientMq, (char *)&resp, attr.mq_msgsize, NULL) != sizeof(struct response))
    fatal("Can't read response from server");

  printf("%d\n",resp.seqNum);

  exit(EXIT_SUCCESS);

}
