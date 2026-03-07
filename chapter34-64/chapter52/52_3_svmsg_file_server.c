#include "52_3_svmsg_file.h"
#include <mqueue.h>


static void grimpReaper(int sig)
{
  int savedErrno;
  savedErrno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0)
    continue;
  errno = savedErrno;
}

static void serveRequest(const struct requestMsg *req)
{
  int fd;
  ssize_t numRead;
  struct responseMsg resp;
  mqd_t clientId;
  char client_name[PATH_MAX];

  sprintf(client_name, CLIENT_FILE,(long) req->clientId);
  if ((clientId = mq_open(client_name, O_WRONLY)) == -1)
    errExit("mq_open");

  fd = open(req->pathname,O_RDONLY);
  if (fd == -1)
  {
    resp.mtype = RESP_MT_FAILURE;
    snprintf(resp.data , sizeof(resp.data), "%s","Coulnd open");

    if (mq_send(clientId, (char *)&resp, sizeof(resp.mtype), 0) == -1)
      errExit("mq_send");
    exit(EXIT_FAILURE);
      }

  resp.mtype = RESP_MT_DATA;
  while ((numRead = read(fd, resp.data, DATA_MSG_SIZE)) > 0)
    if (mq_send(clientId, (char *)&resp, sizeof(resp.mtype), 0) == -1)
      break;

  resp.mtype = RESP_MT_END;
  if (mq_send(clientId, (char *)&resp , sizeof(resp.mtype), 0) == -1)
    errExit("mq_send");
  
}

int main(int argc,char *argv[])
{
  struct requestMsg req;
  pid_t pid;
  ssize_t msgLen;
  mqd_t serverId;
  struct sigaction sa;
  struct mq_attr attr;

  attr.mq_maxmsg = 10;
  attr.mq_msgsize = sizeof(struct requestMsg);
  if ((serverId = mq_open(SERVER_FILE, O_CREAT | O_EXCL | O_RDONLY,S_IRUSR | S_IWUSR,&attr)) == -1)
    errExit("mq_open");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = grimpReaper;
  if (sigaction(SIGCHLD, &sa,NULL) == -1)
    errExit("sigaction");

  for (;;)
    {
      msgLen = mq_receive(serverId, (char *)&req , attr.mq_msgsize, NULL);
      if (msgLen == -1)
        {
          if (errno == EINTR)
            continue;
          errExit("mq_receive");
        }
      pid = fork();
      if (pid == -1)
        {
          errMsg("fork");
          break;
        }
      if (pid == 0)
        {
          serveRequest(&req);
          _exit(EXIT_SUCCESS);
        }
    }
  if (mq_unlink(SERVER_FILE) == -1)
    errExit("mq_unlink");
  exit(EXIT_SUCCESS);
}
