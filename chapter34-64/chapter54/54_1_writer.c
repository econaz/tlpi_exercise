#include "54_1.h"


int main(int argc,char *argv[]){

  int fd,xfrs,bytes;
  void *addr;

  fd = shm_open(SHM_NAME, O_RDWR, 0);
  if (fd == -1)
    errExit("shm_open");

  addr = mmap(NULL, size_t len, int prot, int flags, int fd, __off_t offset)

  for (xfrs = 0,bytes = 0;;xfrs++,bytes += 
 

  

}
