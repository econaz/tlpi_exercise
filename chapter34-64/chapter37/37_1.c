#include "tlpi_hdr.h"
#include <stdlib.h>
#include <sys/syslog.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int level, facility;
  int ch;
  char ident[1024];
  int hasIdent = 0;
  const char *optchar = "hf:i:l:";

  while ((ch = getopt(argc, argv, optchar)) != -1) {
    switch (ch) {
    case 'h':
      printf("%s Usage : [-f facility] [-i ident] [-l level] [-h] <msg> \n",
             argv[0]);
      exit(EXIT_SUCCESS);

    case 'f':
      if (strcmp(optarg, "auth") == 0)
        facility = LOG_AUTH;
      else if (strcmp(optarg, "authpriv") == 0)
        facility = LOG_AUTHPRIV;
      else if (strcmp(optarg, "cron") == 0)
        facility = LOG_CRON;
      else if (strcmp(optarg, "daemon") == 0)
        facility = LOG_DAEMON;
      else if (strcmp(optarg, "ftp") == 0)
        facility = LOG_FTP;
      else if (strcmp(optarg, "localo") == 0)
        facility = LOG_LOCAL0;
      else if (strcmp(optarg, "lpr") == 0)
        facility = LOG_LPR;
      else if (strcmp(optarg, "mail") == 0)
        facility = LOG_MAIL;
      else if (strcmp(optarg, "new") == 0)
        facility = LOG_NEWS;
      else if (strcmp(optarg, "syslog") == 0)
        facility = LOG_SYSLOG;
      else if (strcmp(optarg, "user") == 0)
        facility = LOG_USER;
      else if (strcmp(optarg, "uucp") == 0)
        facility = LOG_UUCP;
      break;

    case 'i':
      strcpy(ident, optarg);
      hasIdent = 1;
      break;

    case 'l':
      if (strcmp(optarg, "emerg") == 0)
        level = LOG_EMERG;
      else if (strcmp(optarg, "alert") == 0)
        level = LOG_ALERT;
      else if (strcmp(optarg, "crit") == 0)
        level = LOG_CRIT;
      else if (strcmp(optarg, "err") == 0)
        level = LOG_ERR;
      else if (strcmp(optarg, "warning") == 0)
        level = LOG_WARNING;
      else if (strcmp(optarg, "notice") == 0)
        level = LOG_NOTICE;
      else if (strcmp(optarg, "info") == 0)
        level = LOG_INFO;
      else if (strcmp(optarg, "debug") == 0)
        level = LOG_DEBUG;
      break;

    default:
      printf("%s Usage : [-f facility] [-i ident] [-l levle] [-h] <msg> \n",
             argv[0]);
      _exit(EXIT_FAILURE);
    }
  }
  if (hasIdent)
    openlog(ident, LOG_PID, facility);
  else
    openlog(0, LOG_PID, facility);

  if (optind == argc - 1)
    syslog(level, "%s", argv[optind]);
  else {
    printf("%s Usage : [-f facility] [-i ident] [-l levle] [-h] <msg> \n",
           argv[0]);
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
