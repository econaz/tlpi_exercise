#include <errno.h>
#include <unistd.h>
#include "59_1.h"

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

  if (rlbuf == NULL) {
    errno = EINVAL;
    return -1;
  }
  numRead = read(rlbuf->fd, rlbuf->buf, BUF_SIZE - 1);
  if (numRead == -1)
    return -1;
  if (numRead == 0)
    return 0;
  rlbuf->buf[numRead++] = '\0';

  return numRead;
}
