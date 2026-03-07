#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"


#define SERVER_FILE "/server_file"
#define CLIENT_FILE "/client_file.%ld"


struct requestMsg{

  long mtype;
  int clientId;
  char pathname[PATH_MAX];
};


#define DATA_MSG_SIZE 8192

struct responseMsg{

  long mtype;
  char data[DATA_MSG_SIZE];
};

#define RESP_MT_FAILURE 1
#define RESP_MT_DATA 2
#define RESP_MT_END 3
