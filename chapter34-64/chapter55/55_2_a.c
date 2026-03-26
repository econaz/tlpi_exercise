#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/file.h>
#include <wait.h>

int main(int argc, char *argv[]) {

  int fd, cfd, lock;
  const char *lname;

  if (argc < 4 || strcmp(argv[1], "--help") == 0 ||
      strchr("sx", argv[3][0]) == NULL)
    usageErr("%s file1 file2 lock [sleep-time]\n", argv[0]);

  lock = (argv[3][0] == 's') ? LOCK_SH : LOCK_EX;
  if (argv[3][0] == 'n')
    lock |= LOCK_NB;

  /* fd = open(argv[1], O_RDONLY); */
  /* if (fd == -1) */
  /*   errExit("open"); */

  /* cfd = open(argv[2], O_RDONLY); */
  /* if (cfd == -1) */
  /*   errExit("open"); */

  lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

  printf("PID %ld: requesting %s \n", (long)getpid(), lname);

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
      errExit("open");
    cfd = open(argv[2], O_RDONLY);
    if (cfd == -1)
      errExit("open");
    printf("child start\n");
    
    if (flock(cfd, lock) == -1) {
      if (errno == EWOULDBLOCK)
        fatal("PID %ld: arleady locked -bye!", (long)getpid());
      else
        errExit("flock (PID=%ld)", (long)getpid());
    }


    if (flock(fd, lock) == -1) {
      if (errno == EWOULDBLOCK)
        fatal("PID %ld: arleady locked -bye!", (long)getpid());
      else
        errExit("flock (PID=%ld)", (long)getpid());
    }
    
    if (flock(cfd, LOCK_UN) == -1)
      errExit("flock un");
    if (flock(fd, LOCK_UN) == -1)
      errExit("flock un");

    sleep(10);
    _exit(EXIT_SUCCESS);

  default:
    printf("parent start\n");


    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
      errExit("open");
    cfd = open(argv[2], O_RDONLY);
    if (cfd == -1)
      errExit("open");
    
    if (flock(fd, lock) == -1) {
      if (errno == EWOULDBLOCK)
        fatal("PID %ld: locked -bye!", (long)getpid());
      else
        errExit("flock (PID=%ld)", (long)getpid());
    }
    sleep((argc > 4) ? getInt(argv[4], GN_NONNEG, "sleep-time") : 10);
    
    if (flock(cfd, lock) == -1) {
      if (errno == EWOULDBLOCK)
        fatal("PID %ld:locked -bye!", (long)getpid());
      else
        errExit("flock (PID=%ld)", (long)getpid());
    }
    
    if (flock(fd, LOCK_UN) == -1)
      errExit("flock_un");
    if (flock(cfd, LOCK_UN) == -1)
      errExit("flock_un");

    printf("parent unlock prepare exit\n");

    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}
