#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"



int main(int argc,char *argv[])
{

  int fd;
  struct stat st;
  char *addr,*buf;
  long ps;
  


  ps = sysconf(_SC_PAGESIZE);
  fd = open("a", O_CREAT | O_RDWR , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (fd == -1)
    errExit("open");
  
  if (ftruncate(fd, ps+200) == -1)
    errExit("ftruncate");

  addr = mmap(NULL,ps+200,PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED)
    errExit("mmap");
  
  buf = calloc(ps, sizeof(char));
  if (buf == NULL)
    errExit("calloc");

  memcpy(addr, buf, ps);

  if (mmap(addr,ps,PROT_READ | PROT_WRITE,MAP_FIXED,fd,0) == MAP_FAILED)
    errExit("mmap");

  if (mmap(addr+ps, ps, PROT_WRITE | PROT_READ, MAP_FIXED, fd, ps) == MAP_FAILED)
    errExit("mmap");

  if (close(fd) == -1)
    errExit("close");

  exit(EXIT_SUCCESS);
  
}
