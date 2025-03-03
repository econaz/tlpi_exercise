#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

static void x() {
  if (setjmp(env) == 0) {
    printf("x1\n");
  } else {
    printf("x2\n");
  }
  printf("x\n");
}

static void y() {
  longjmp(env, 1);
  printf("y\n");
}

int main(int argc, char *argv[]) {
  x();
  y();
  printf("main\n");

  exit(EXIT_SUCCESS);
}
