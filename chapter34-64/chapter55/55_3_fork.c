#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/file.h>
#include <wait.h>

//fork 会使用同一把锁
//所以child 和 parent 都可以运行不会阻塞
int main(int argc, char *argv[]) {

  int fd, cfd, lock;
  const char *lname;

  if (argc < 3 || strcmp(argv[1], "--help") == 0 ||
      strchr("sx", argv[2][0]) == NULL)
    usageErr("%s file lock [sleep-time]\n", argv[0]);

  lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;

  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    errExit("open");

  lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

  printf("PID %ld: requesting %s\n", (long)getpid(), lname);

  switch (fork()) {
  case -1:
    errExit("fork");
  case 0:
    printf("child start\n");
    if (flock(fd, lock) == -1)
      errExit("flock (PID=%ld)", (long)getpid());
    printf("child start sleep\n");
    sleep(100);
    if (flock(fd, LOCK_UN) == -1)
      errExit("flock_un");
    _exit(EXIT_SUCCESS);
  default:
    break;
  }
  printf("parent start\n");
  if (flock(fd, lock) == -1)
    errExit("flock (PID=%ld)", (long)getpid());
  printf("parent got flock\n");
  if (flock(fd, LOCK_UN) == -1)
    errExit("flock_un");
  wait(NULL);
  exit(EXIT_SUCCESS);
}
