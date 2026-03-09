#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


#define NOTIFY SIGUSR1


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
  sigset_t blockMask;
  struct sigaction sa;
  siginfo_t info;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s mq-name\n",argv[0]);
  mqd = mq_open(argv[1],O_RDONLY | O_NONBLOCK);
  if (mqd == (mqd_t) -1)
    errExit("mq-open");

  if (mq_getattr(mqd, &attr) == -1)
    errExit("mq_getattr");

  buffer = malloc(attr.mq_msgsize);
  if (buffer == NULL)
    errExit("malloc");

  sigemptyset(&blockMask);
  sigaddset(&blockMask, NOTIFY);
  if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
    errExit("sigpromask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(NOTIFY, &sa, NULL) == -1)
    errExit("sigation");

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = NOTIFY;
  if (mq_notify(mqd, &sev) == -1)
    errExit("mq_notify");

  for (;;)
    {
      if (sigwaitinfo(&blockMask, &info) == -1)
        errExit("sigwaitinfo");

      printf("si_mesgq = %d,si_code= %d,signo = %d\n",SI_MESGQ,info.si_code,info.si_signo);

      if (mq_notify(mqd, &sev) == -1)
        errExit("mq_notify");

      while ((numRead = mq_receive(mqd, buffer, attr.mq_msgsize,NULL)) >= 0)
        printf("Read %ld bytes\n",(long) numRead);

      if (errno != EAGAIN)
        errExit("mq_receive");
      
  
    }

}
