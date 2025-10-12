#include "tlpi_hdr.h"

#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int pipeFd[2], pipe2Fd[2];
  char data[50];
  int n;

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
      n = read(pipeFd[0], data, 50);
      if (n == -1)
        errExit("child read");

      // if (n == 0)
      //   break;
      printf("child get data from parent - %s\n", data);

      for (int i = 0; i < strlen(data); i++)
        data[i] = toupper(data[i]);

      sleep(3);

      if (write(pipe2Fd[1], data, strlen(data) * sizeof(char)) !=
          strlen(data) * sizeof(char))
        errExit("child write");
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

  for (;;) {

    size_t size;
    size = (strlen(data) - 1) * sizeof(char);

    if (write(pipeFd[1], data, size) != size)
      errExit("parent write");

    sleep(3);

    if (read(pipe2Fd[0], data, size) != size)
      errExit("parent read");

    printf("parent get data from child- %s\n", data);

    for (int i = 0; i < strlen(data); i++)
      data[i] = tolower(data[i]);
  }

  exit(EXIT_SUCCESS);
}
