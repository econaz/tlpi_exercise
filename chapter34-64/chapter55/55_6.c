#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_LINE 100

static void displayCmdFmt(void) {

  printf("\n   Format:cmd lock start length [where]\n\n");
  printf("     'cmd' is 'g' (GETLK),'s' (SETLK),or 'w' (SETLKW)\n");
  printf("     'lock' is 'r' (READ),'w' (WRITE),or 'u' (UNLOCK)\n");
  printf("     'start' and 'length' specify byte range to lock\n");
  printf("     'whence' is 's' (SEEK_SET,default),'c' (SEEK_CUR), or 'e' "
         "(SEEK_END)\n\n");
}

int main(int argc, char *argv[]) {

  int fd, numRead, cmd, status;
  char lock, cmdCh, whence, line[MAX_LINE];
  struct flock fl;
  long long len, st;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file\n", argv[0]);

  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    errExit("open (%s)", argv[1]);

  printf("Enter ? for help\n");

  for (;;) {

    printf("PID=%ld> ", (long)getpid());
    fflush(stdout);

    if (fgets(line, MAX_LINE, stdin) == NULL)
      exit(EXIT_SUCCESS);

    line[strlen(line) - 1] = '\0';

    if (*line == '\0')
      continue;

    if (line[0] == '?'){
      displayCmdFmt();
      continue;
    }

    whence = 's';
    numRead = sscanf(line, "%c %c %lld %lld %c",&cmdCh,&lock,&st,&len,&whence);

    fl.l_start = st;
    fl.l_len = len;

    if (numRead < 4 || strchr("gsw", cmdCh) == NULL || strchr("rwu", lock) == NULL || strchr("sce", whence) == NULL){
      printf("Invalid command!\n");
      continue;
    }

    cmd = (cmdCh == 'g') ? F_GETLK : (cmdCh == 's') ? F_SETLK : F_SETLKW;
    fl.l_type = (lock == 'r') ? F_RDLCK : (lock == 'w') ? F_WRLCK : F_UNLCK;
    fl.l_whence = (whence == 'c') ? SEEK_CUR : (whence == 'e') ? SEEK_END : SEEK_SET;
    status = fcntl(fd,cmd,&fl);

    if (cmd == F_GETLK){
      if (status == -1){
        errMsg("fcntl - F_GETLK");
      }else{
        if (fl.l_type == F_UNLCK)
          printf("[PID=%ld] lock can be placed\n",(long)getpid());
        else
          printf("[PID=%ld] Denied by %s lock on %lld:%lld (held by PID %ld)\n",(long)getpid(),(fl.l_type == F_RDLCK) ? "READ": "WRITE",(long long) fl.l_start,(long long) fl.l_len,(long)fl.l_pid);
      }

    }else {
      if (status == 0)
        printf("[PID=%ld] %s\n",(long)getpid(),(lock == 'u') ? "unlocked" : "got lock");
      else if (errno == EAGAIN || errno == EACCES)
        printf("[PID=%ld] failed (incompatible lock)\n",(long)getpid());
      else if (errno == EDEADLK)
        printf("[PID=%ld] failed (deadlock)\n",(long)getpid());
      else
        errMsg("fcntl - F_SETLK(W)");

    }
  }
}
