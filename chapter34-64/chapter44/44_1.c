#include "tlpi_hdr.h"

#include <ctype.h>
#include <error_functions.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int pipeFd[2], pipe2Fd[2];
  char data[50];

  if (pipe(pipeFd) == -1)
    errExit("pipe");

  if (pipe(pipe2Fd) == -1)
    errExit("pipe2Fd");

  switch (fork()) {
  case -1:
    errExit("fork");

  case 0:

    for (;;) {
      if (close(pipeFd[1]) == -1)
        errExit("child close parent write pipe");
      if (close(pipe2Fd[0]) == -1)
        errExit("child close child read pipe ");

      if (read(pipeFd[0], data, sizeof(data)) != sizeof(data))
        fatal("child read failed");

      for (int i = 0; i < strlen(data); i++)
        data[i] = toupper(data[i]);

      sleep(3);

      if (write(pipe2Fd[1], data, sizeof(data)) != sizeof(data))
        fatal("child write failed");
    }
    _exit(EXIT_SUCCESS);

  default:
    break;
  }

  while (1) {

    if (close(pipe2Fd[1]) == -1)
      errExit("parent close child write pipe");

    if (close(pipeFd[0]) == -1)
      errExit("parent close parent read pipe");

    if (fgets(data, 50, stdin) != NULL)
      break;
    continue;
  }

  printf("%s\n", data);

  printf("%ld\n", strlen(data));

  printf("%lu\n", sizeof(data));

  for (;;) {

    if (write(pipeFd[1], data, sizeof(data)) != sizeof(data))
      fatal("parent write failed");

    sleep(3);

    if (read(pipe2Fd[0], data, sizeof(data)) != sizeof(data))
      fatal("parent read failed");
  }

  exit(EXIT_SUCCESS);
}
