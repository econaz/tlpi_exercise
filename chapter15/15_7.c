
#include <fcntl.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
// void resolve(const char *attr) {
//
//   int newattr = 0;
//
//   for (int i = 0; attr[i] != '\0'; i++) {
//
//     switch (attr[i]) {
//       case '+':
//
//     }
//   }
// }
int mychattr(const char *cmd, char *filename, int attr) {

  int newattr = 0, sign;

  sign = cmd[0];
  for (int i = 1; cmd[i] != '\0'; i++) {
    switch (cmd[i]) {
    case 'a':
      newattr |= FS_APPEND_FL;
      break;
    case 'A':
      newattr |= FS_NOATIME_FL;
      break;
    case 'c':
      newattr |= FS_COMPRBLK_FL;
      break;
    case 'd':
      newattr |= FS_NODUMP_FL;
      break;
    case 'D':
      newattr |= FS_DIRSYNC_FL;
      break;
    case 'i':
      newattr |= FS_IMMUTABLE_FL;
      break;
    case 'j':
      newattr |= FS_JOURNAL_DATA_FL;
      break;
    case 's':
      newattr |= FS_SECRM_FL;
      break;
    case 'S':
      newattr |= FS_SYNC_FL;
      break;
    case 't':
      newattr |= FS_NOATIME_FL;
      break;
    case 'T':
      newattr |= FS_TOPDIR_FL;
      break;
    case 'u':
      newattr |= FS_UNRM_FL;
      break;
    default:
      break;
    }
  }
  switch (sign) {
  case '+':
    attr |= newattr;
    break;
  case '-':
    attr &= (~newattr);
    break;
  case '=':
    attr = newattr;
    break;
  default:
    break;
  }
  return attr;
}

int main(int argc, char *argv[]) {
  int fd, attr;
  for (int i = 2; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) == -1) {
      printf("error open");
      exit(EXIT_FAILURE);
    }
    if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
      printf("error ioctl get");
      close(fd);
      exit(EXIT_FAILURE);
    }
    attr = mychattr(argv[1], argv[i], attr);
    printf("%d\n", attr);
    if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
      printf("error ioctl set");
      close(fd);
      exit(EXIT_FAILURE);
    }
    close(fd);
  }
  exit(EXIT_SUCCESS);
}
