#include "become_daemon.h"
#include "inet_sockets.h"
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/wait.h>
#include <syslog.h>

#define SERVICE "echo"
#define BUF_SIZE 4096

#define SEM_NAME "/echo_sv_sem"
#define MAX_CONN 10

static sem_t *sem;

static void grimReaper(int sig) {

  int savedErrno;

  savedErrno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0) {

    if (sem_post(sem) == -1) {

      syslog(LOG_ERR, "Failed in sem_post");
      exit(EXIT_FAILURE);
    }
  }
  errno = savedErrno;
}

static void handleRequest(int cfd) {

  char buf[BUF_SIZE];
  ssize_t numRead;

  while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {

    if (write(cfd, buf, numRead) != numRead) {
      syslog(LOG_ERR, "write() failed");
      exit(EXIT_FAILURE);
    }
  }
  if (numRead == -1) {
    syslog(LOG_ERR, "Error from read():%s", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {

  int lfd, cfd;
  struct sigaction sa;

  if (becomeDaemon(0) == -1)
    errExit("becomDaemon");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = grimReaper;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    syslog(LOG_ERR, "Error from sigaction(): %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  lfd = inetListen(SERVICE, 10, NULL);

  if (lfd == -1) {
    syslog(LOG_ERR, "Cloud not create server socket (%s)", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IWUSR | S_IRUSR,
                      MAX_CONN)) == SEM_FAILED) {
    syslog(LOG_ERR, "Could not create sem (%s)", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (sem_unlink(SEM_NAME) == -1) {
    syslog(LOG_ERR, "Could not unlink (%s)", strerror(errno));
  }

  for (;;) {

    if (sem_wait(sem) == -1) {

      syslog(LOG_ERR, "Failed in sem_wait():%s", strerror(errno));
      exit(EXIT_FAILURE);
    }

    cfd = accept(lfd, NULL, NULL);
    if (cfd == -1) {
      syslog(LOG_ERR, "Failed in accept(): %s", strerror(errno));
      exit(EXIT_FAILURE);
    }

    switch (fork()) {
    case -1:
      syslog(LOG_ERR, "Can't create child (%s)", strerror(errno));
      close(cfd);
      break;

    case 0:
      close(lfd);
      handleRequest(cfd);
      _exit(EXIT_SUCCESS);

    default:
      close(cfd);
      break;
    }
  }
}
