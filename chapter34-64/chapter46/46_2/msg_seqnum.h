#ifndef MSG_SEQNUM_H
#define MSG_SEQNUM_H

#include <stddef.h>
#include <unistd.h>

#define SERVER_FILE "/tmp/server_file_msg_seqnum_h"

struct RequestMsg {

  long mtype;
  pid_t pid;
  int seqLen;
};

struct ResponseMsg {
  long mtype;
  int seqNum;
};

#define REQUEST_SIZE                                                           \
  (offsetof(struct RequestMsg, seqLen) - offsetof(struct RequestMsg, pid) +    \
   sizeof(int))

#define RESPONSE_SIZE (sizeof(int))

#endif
