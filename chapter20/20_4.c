#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) { siginterrupt(SIGTERM, 1); }
