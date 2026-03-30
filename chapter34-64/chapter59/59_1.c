#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 4096

struct rlbuf {
  int len;
  char buf[BUF_SIZE];
  int next;
  int fd;
};

int readLineBufInit(int fd, struct rlbuf *buf) {
  buf->len = 0;
  buf->fd = fd;
  buf->next = 0;

  return 0;
}

int readLineBuf(struct rlbuf *rlbuf) {

  ssize_t numRead;
  ssize_t tolRead = 0;
  char buff[BUF_SIZE];
  int n = 1024;

  if (rlbuf == NULL) {
    errno = EINVAL;
    return -1;
  }

  for (;;) {

    numRead = read(rlbuf->fd, buff, n);
    if (numRead == -1) {
      if (errno == EINTR)
        continue;
      else
        return -1;
    } else if (numRead == 0) {
      if (tolRead == 0)
        return 0;
      else
        break;
    }
    else {
      tolRead += numRead;
      if (tolRead > BUF_SIZE){
        
      }
    }
  }
  
}
