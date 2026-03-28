#include <sys/socket.h>
#include <sys/un.h>
#include "tlpi_hdr.h"


#define SOCK_PATH "socket"


struct request{
  pid_t pid;
  int seqLen;
};

struct response{
  int seqNum;
};
