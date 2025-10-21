#include "tlpi_hdr.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[]) {
  int id, j, msqid;
  struct msqid_ds ds;
  struct msginfo msginfo;

  id = msgctl(0, MSG_INFO, (struct msqid_ds *)&msginfo);
  if (id == -1)
    errExit("MSG_INFO");

  for (j = 0; j <= id; j++) {
    msqid = msgctl(j, MSG_STAT, &ds);
    if (msqid == -1) {
      if (errno != EINVAL && errno != EACCES)
        errExit("MSG_STAT");
      continue;
    }

    printf("msqid = %d\n", msqid);
    printf("index = %d seq = %ld cal = %ld\n", j, (long)ds.msg_perm.__seq,
           j + (long)ds.msg_perm.__seq * 32768);

    printf("----------------------\n");
  }

  exit(EXIT_SUCCESS);
}
