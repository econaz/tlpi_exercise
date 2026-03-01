#include <sys/resource.h>
#include <sys/mman.h>
#include "tlpi_hdr.h"


int main(int argc,char *argv[]){

  struct rlimit oldLimit,newLimit;
  char *addr;
  int pageSize;

  if (getrlimit(RLIMIT_MEMLOCK, &oldLimit) == -1)
    errExit("getrlimit");

  printf("%ld,%ld\n",oldLimit.rlim_cur,oldLimit.rlim_max);

  newLimit.rlim_cur = 1024 * 20;
  newLimit.rlim_max = 1024 * 20;

  if (setrlimit(RLIMIT_MEMLOCK, &newLimit) == -1)
    errExit("setrlimit");

  if (getrlimit(RLIMIT_MEMLOCK, &newLimit) == -1)
    errExit("getrlimit");

  printf("after %ld,%ld\n",newLimit.rlim_cur,newLimit.rlim_max);
  
  pageSize = sysconf(_SC_PAGESIZE);
  if (pageSize == -1)
    errExit("sysconf(_SC_PAGESIZE)");

  addr = mmap(NULL, pageSize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

  if (addr == MAP_FAILED)
    errExit("mmap");

  
  if (mlock(addr, 20 * 1024 + 1) == -1)
    errExit("mlock");

  exit(EXIT_SUCCESS);
}
