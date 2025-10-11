#include "tlpi_hdr.h"

#include <ctype.h>
#include <error_functions.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
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

    if (close(pipe2Fd[0]) == -1)
      errExit("child close child read pipe ");

    if (close(pipeFd[1]) == -1)
      errExit("child close parent write pipe");

    for (;;) {
      read(pipeFd[0], data, sizeof(data));

      printf("child get data from parent - %s\n", data);
      for (int i = 0; i < strlen(data); i++)
        data[i] = toupper(data[i]);

      sleep(3);

      write(pipe2Fd[1], data, sizeof(data));
    }
    _exit(EXIT_SUCCESS);

  default:
    break;
  }
  if (close(pipe2Fd[1]) == -1)
    errExit("parent close child write pipe");

  if (close(pipeFd[0]) == -1)
    errExit("parent close parent read pipe");

  while (1) {

    if (fgets(data, 50, stdin) != NULL)
      break;
    continue;
  }

  printf("%s\n", data);

  printf("%ld\n", strlen(data));

  printf("%lu\n", sizeof(data));

  for (;;) {

    write(pipeFd[1], data, sizeof(data));

    sleep(3);

    read(pipe2Fd[0], data, sizeof(data));

    printf("parent get data from child- %s\n", data);
  }

  exit(EXIT_SUCCESS);
}
