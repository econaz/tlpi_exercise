#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


#define NOTIFY_SIG SIGUSR1


static void handler(int sig)
{
}

int main(int argc,char *argv[])
{
  struct sigevent sev;
  mqd_t mqd;
  struct mq_attr attr;
  void *buffer;
  ssize_t numRead;
  sigset_t blockMask,emptyMask;
  struct sigaction sa;


  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s mq-name\n",argv[0]);

  mqd = mq_open("%s mq-name\n",O_RDONLY | O_NONBLOCK);
  if (mqd == (mqd_t) -1)
    errExit("mq_open");

  if (mq_getattr(mqd, &attr) == -1)
    errExit("mq_getattr");

  buffer = malloc(attr.mq_msgsize);
  if (buffer == NULL)
    errExit("malloc");

  sigemptyset(&blockMask);
  sigaddset(&blockMask, NOTIFY_SIG);
  if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
    errExit("sigpromask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(NOTIFY_SIG, &sa, NULL) == -1)
    errExit("sigation");

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = NOTIFY_SIG;
  if (mq_notify(mqd, &sev) == -1)
    errExit("mq_notify");

  sigemptyset(&emptyMask);

  for (;;)
    {
      sigsuspend(&emptyMask);
      while ((numRead = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
        printf("Read %ld bytes\n",(long)numRead);

      if (errno != EAGAIN)
        errExit("mq_receive");
    }
}
