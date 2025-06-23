#include <string.h>
#include <sys/stat.h>
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

int add_to_watch(char *path, int inotofityFd) {

  int mask = IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM;
  inotify_add_watch(inotofityFd, path, mask);
  return 0;
}

int add_inotify(const char *fpath, struct stat *sb, int typeflag,
                struct FTW *ftwbuf) {

  if (S_ISDIR(sb->st_mode)) {
    add_to_watch(fpath, int inotofityFd)
  }
}

int main(int argc, char *argv[]) {
  int inotifyFd, wd, j;
  char buf[BUF_LEN];
  ssize_t numRead;
  char *p;
  struct inotify_event *event;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname... \n", argv[0]);

  inotifyFd = inotify_init();
  if (inotifyFd == -1)
    errExit("inotify_event");

  for (j = 1; j < argc; j++) {
    wd = inotify_add_watch(inotifyFd, argv[j],
                           IN_CREATE | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE);
    if (wd == -1)
      errExit("inotify_add_watch");

    printf("Watching %s using wd %d\n", argv[j], wd);
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
