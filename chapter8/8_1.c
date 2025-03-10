
#include <pwd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct passwd *p1, *p2;

  p1 = getpwnam("root");
  // not reentrant
  p2 = getpwnam("bin");

  printf("%ld %ld", (long)p1->pw_uid, (long)p2->pw_uid);
  return 0;
}
