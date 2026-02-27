#include <sys/mman.h>
#include <signal.h>
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>



#define BUF_TIMES 3000

void handler(int sig)
{
  printf("Got SIGBUS\n");
  exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[]){

  int fd;
  struct stat st;
  struct sigaction sa;
  char *addr;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGBUS,&sa,NULL) == -1)
    errExit("sigaction");

  fd = open("a",O_CREAT | O_RDWR,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (fd == -1)
    errExit("open");

  char *me =(char*) calloc(BUF_TIMES,sizeof(char));
  if (me == NULL)
    errExit("calloc");

  if (write(fd,me,BUF_TIMES) != BUF_TIMES)
    errExit("write");

  free(me);

  if (fstat(fd,&st) == -1)
    errExit("fstat");

  printf("file size is %ld\n",st.st_size);

  //PAGE_SIZE 4096,malloc size 8000;
  addr = mmap(NULL,8000,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
  if (addr == MAP_FAILED)
    errExit("mmap");

  printf("go to mem 4500,%d\n",*(addr+4500));

  printf("go to mem 6000,%d\n",*(addr+10000)); 
  
}
