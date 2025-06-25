#define _XOPEN_SOURCE 600
#include <ftw.h>
#include <limits.h>
#include <sys/inotify.h>
#include <tlpi_hdr.h>

static void displayInotifyEvent(struct inotify_event *i) {
  printf("     wd =%2d; ", i->wd);
  if (i->cookie > 0)
    printf("cookie =%4d;  ", i->cookie);

  printf("mask = ");

  if (i->mask & IN_CREATE)
    printf("IN_CREATE ");
  if (i->mask & IN_DELETE)
    printf("IN_DELETE ");
  if (i->mask & IN_MOVED_FROM)
    printf("IN_MOVED_FROM");
  if (i->mask & IN_MOVED_TO)
    printf("IN_MOVED_TO");
  printf("\n");

  if (i->len > 0)
    printf("       name = %s\n", i->name);
}
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static int inotifyFd;

int add(const char *fpath, const struct stat *sb, int typeflag,
        struct FTW *ftwbuf) {

  int mask = IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM;
  int wd;
  if (S_ISDIR(sb->st_mode)) {
    wd = inotify_add_watch(inotifyFd, fpath, mask);
    if (wd == -1)
      errExit("inotify_add_watch");
    printf("Watching %s using wd %d\n", fpath, wd);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int wd, j, flag = 0;
  char buf[BUF_LEN];
  ssize_t numRead;
  char *p;
  struct inotify_event *event;
  flag |= FTW_PHYS;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname... \n", argv[0]);

  inotifyFd = inotify_init();
  if (inotifyFd == -1)
    errExit("inotify_event");

  for (j = 1; j < argc; j++) {
    if (nftw(argv[j], add, 10, flag) == -1)
      errExit("nftw");
  }
  for (;;) {
    numRead = read(inotifyFd, buf, BUF_LEN);
    if (numRead == 0)
      fatal("read() from inotify fd returned 0!");

    if (numRead == -1)
      errExit("read");

    printf("Read %ld bytes from inotyfy fd\n", (long)numRead);

    for (p = buf; p < buf + numRead;) {
      event = (struct inotify_event *)p;
      displayInotifyEvent(event);
      p += sizeof(struct inotify_event) + event->len;
    }
  }
  exit(EXIT_SUCCESS);
}
