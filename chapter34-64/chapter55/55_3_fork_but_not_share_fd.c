#include <fcntl.h>
#include <sys/file.h>
#include "tlpi_hdr.h"

//not share fd
//if child got flock first,parent will block;
int main(int argc,char *argv[]){


  int fd,lock;
  const char *lname;

  if (argc < 3 || strcmp(argv[1], "--help") == 0 || strchr("sx", argv[2][0]) == NULL)
    usageErr("%s file lock [sleep-time]\n",argv[0]);

  lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;

  lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

  printf("pid = %ld,lock = %s\n",(long)getpid(),lname);

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    printf("child start\n");
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
      errExit("child open");
    if (flock(fd,lock) == -1)
      errExit("child flock");
    printf("child got lock ");
    printf("child start sleep\n");
    sleep(20);
    if (flock(fd, LOCK_UN) == -1)
      errExit("child lock_un");
    _exit(EXIT_SUCCESS);

    default:
      break;
      }

  printf("parent start\n");
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    errExit("parent open\n");
  
  if (flock(fd, lock) == -1)
    errExit("parent flock\n");
  printf("parent got flock\n");

  if (flock(fd, LOCK_UN) == -1)
    errExit("parent lock_un\n");
  exit(EXIT_SUCCESS);
  
}
