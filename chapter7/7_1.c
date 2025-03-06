#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {

  printf("Initial progam break is: %10p\n", sbrk(0));

  char *cp;
  int i;

  for (i = 0; i < 10; i++) {
    cp = (char *)malloc(102400);
    printf("Program break is: %10p\n", sbrk(0));
  }
  // Initial progam break is: 0x62669d1b7000
  // Program break is: 0x62669d1e2000
  // Program break is: 0x62669d1e2000
  // Program break is: 0x62669d214000
  // Program break is: 0x62669d214000
  // Program break is: 0x62669d246000
  // Program break is: 0x62669d246000
  // Program break is: 0x62669d278000
  // Program break is: 0x62669d278000
  // Program break is: 0x62669d2aa000
  // Program break is: 0x62669d2aa000
}
