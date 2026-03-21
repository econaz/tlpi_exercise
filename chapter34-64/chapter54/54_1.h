#include "tlpi_hdr.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>



#define SHM_NAME "/SHM_DEMO"

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)



#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif



static sem_t rSem;
static sem_t wSem;
static volatile ssize_t num = 0;
