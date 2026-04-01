#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 4096

struct rlbuf {
  int len;
  char buf[BUF_SIZE];
  int next;
  int fd;
};

int readLineBufInit(int fd, struct rlbuf *buf); 

int readLineBuf(struct rlbuf *rlbuf);
