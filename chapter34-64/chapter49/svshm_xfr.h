#include "binary_sems.h"
#include "tlpi_hdr.h"
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

//#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define TEMP_FILE  "/tmp/test"

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM 0
#define READ_SEM 1

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

struct shmseg {
  int cnt;
  char buf[BUF_SIZE];
};
