#include "tlpi_hdr.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>



#define SHM_NAME "/SHM_DEMO"

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM "/WRITE_SEM"
#define READ_SEM  "/READ_SEM"


#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif



static sem_t *rSem;
static sem_t *wSem;

struct shmseg
{
  int cnt;
  char buf[BUF_SIZE];
};
