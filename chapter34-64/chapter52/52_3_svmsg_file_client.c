#include <mqueue.h>
#include "52_3_svmsg_file.h"


char client_name[PATH_MAX];


static void removeQueue(void)
{
  mq_unlink(client_name);
}

int main(int argc,char *argv[])
{
  struct requestMsg req;
  struct responseMsg resp;
  int numMsg;
  mqd_t serverMq,clientMq;
  struct mq_attr attr;
  ssize_t msgLen,toBytes;

  if (argc != 2 || strcmp(argv[1],"--help") == 0)
    usageErr("%s pathname\n",argv[0]);

  if (strlen(argv[1]) > sizeof (req.pathname) - 1)
    cmdLineErr("pathanem too long (max: %ld bytes)\n",(long) sizeof(req.pathname) - 1);

  if ((serverMq = mq_open(SERVER_FILE, O_WRONLY)) == -1)
    errExit("mq_open");

  attr.mq_maxmsg = 10;
  attr.mq_msgsize = sizeof(struct responseMsg);

  sprintf(client_name, CLIENT_FILE,(long)getpid());

  if ((clientMq = mq_open(client_name, O_CREAT | O_EXCL | O_RDONLY,S_IRUSR | S_IRWXU,&attr)) == -1)
    errExit("mq_open");

  if (atexit(removeQueue) != 0)
    errExit("atexit");

  strncpy(req.pathname, argv[1], sizeof(req.pathname) -1);

  req.pathname[sizeof(req.pathname) -1] = '\0';
  req.clientId = getpid();

  if (mq_send(serverMq, (char *)&req, sizeof(req.mtype), 0) == -1)
    errExit("mq_send");

  if ((msgLen = mq_receive(clientMq, (char *)&resp, attr.mq_msgsize, NULL)) == -1)
    errExit("mq_receive");

  if (resp.mtype == RESP_MT_FAILURE)
    {
      printf("%s\n",resp.data);
      exit(EXIT_FAILURE);
    }

  toBytes = msgLen - sizeof(resp.mtype);
  for (numMsg = 1; resp.mtype == RESP_MT_DATA;numMsg++){

    if ((msgLen = mq_receive(clientMq, (char *)&resp, attr.mq_msgsize, NULL)) == -1)
    errExit("mq_reveive");

    toBytes += msgLen - sizeof(resp.mtype);

  }

printf("Received %ld bytes (%d messages\n",(long)toBytes);

exit(EXIT_SUCCESS);
}
