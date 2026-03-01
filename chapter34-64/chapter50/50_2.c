#include <sys/mman.h>
#include "tlpi_hdr.h"


int main(int argc,char *argv[])
{

  char *addr;

  addr = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (addr == MAP_FAILED)
    errExit("mmap");

  printf("before %d\n",*addr);
  
  if (madvise(addr, 4096, MADV_DONTNEED) == -1)
    errExit("madvise");

  printf("after %d\n",*addr);

  exit(EXIT_SUCCESS);
 
}
