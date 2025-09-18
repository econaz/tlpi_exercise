#include "tlpi_hdr.h"
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  char *passwd, *username, *p, *encrypted;
  struct passwd *pwd;
  struct spwd *spwd;
  int opt = 0;

  while ((opt = getopt(argc, argv, "hu:")) != -1) {
    switch (opt) {

    case 'h':
      printf("Useage: %s [-u username] <program> [arg1 arg2 ...]", argv[0]);
      exit(EXIT_SUCCESS);

    case 'u':
      username = optarg;
      pwd = getpwnam(username);
      if (pwd == NULL)
        fatal("cloudnt not get password!");

      spwd = getspnam(username);
      if (spwd == NULL && errno == EACCES)
        fatal("no perssion to read shadow password");

      if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;

      passwd = getpass("Password:");

      encrypted = crypt(passwd, pwd->pw_passwd);
      for (p = encrypted; *p != '\0';)
        *p++ = '\0';

      if (encrypted == NULL)
        errExit("crypt");

      if (strcmp(encrypted, pwd->pw_passwd) == 1) {
        printf("password error!");
        exit(EXIT_FAILURE);
      }

      printf("login success!");

      if (setreuid(getuid(), getuid()) == -1)
        errExit("setreuid");
      break;

    default:
      printf("Useage: %s [-u username] <program> [arg1 arg2 ...]", argv[0]);
      exit(EXIT_SUCCESS);
    }
  }

  if (optind > argc - 1) {
    printf("Useage: %s [-u username] <program> [arg1 arg2 ...]", argv[0]);
    exit(EXIT_SUCCESS);
  }

  char *argsv[128];
  int i = 0;
  for (; optind + i < argc; i++) {
    argsv[i] = argv[optind + i];
  }
  argsv[i] = NULL;

  if (execv(argv[optind], argsv) == -1)
    errExit("execv");

  exit(EXIT_SUCCESS);
}
