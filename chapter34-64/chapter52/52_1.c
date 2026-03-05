#define _XOPEN_SOURCE 600
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


static void usageError(const char *progName)
{
  fprintf(stderr, "Usage: %s [-n] name\n",progName);
  fprintf(stderr, " -n  Use O_NONBLOCK flag\n");
  exit(EXIT_SUCCESS);

}

int main(int argc,char *argv[])
{
  int flags,opt;
  mqd_t mqd;
  unsigned int prio;
  void *buffer;
  struct mq_attr attr;
  ssize_t readNum;
  struct timespec ts;

  flags = O_RDONLY;

  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    errExit("clock_gettime");

  while ((opt = getopt(argc,argv,"nt")) != -1){

    switch (opt){
    case 'n':
      flags |= O_NONBLOCK;
      break;
    case 't':
      ts.tv_sec += atoi(argv[opt]);
      break;
      default:
        usageError(argv[0]);
    }
  }
  
  if (optind > argc)
    usageError(argv[0]);

  mqd = mq_open(argv[optind], flags);
  if (mqd == (mqd_t) -1)
    errExit("mq_open");

  if (mq_getattr(mqd, &attr) == -1)
    errExit("mq_getattr");

  buffer = malloc(attr.mq_msgsize);
  if (buffer == NULL)
    errExit("malloc");

  readNum = mq_timedreceive(mqd, buffer, attr.mq_msgsize, &prio, &ts);
  if (readNum == -1)
    errExit("mq_timedreceive");

  if (write(STDOUT_FILENO, buffer, readNum) == -1)
    errExit("write");

  write(STDOUT_FILENO, "\n", 1);
  exit(EXIT_SUCCESS);

}
  
