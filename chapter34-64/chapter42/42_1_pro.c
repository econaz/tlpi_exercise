#include "tlpi_hdr.h"
#include <dlfcn.h>
#include <error_functions.h>

void handler(void *handler, const char *name) {

  void (*funcp)(void);
  char *err;

  (void)dlerror();
  *(void **)(&funcp) = dlsym(handler, name);
  err = dlerror();
  if (err != NULL)
    fatal("dlsym:%s", err);

  if (funcp == NULL)
    printf("%s is NULL\n", name);
  else
    (*funcp)();
}

int main(int argc, char *argv[]) {
  void *libHandleA, *libHandleB;

  libHandleA = dlopen("./liba.so", RTLD_NOW | RTLD_GLOBAL);
  if (libHandleA == NULL)
    fatal("dlopen: %s", dlerror());

  libHandleB = dlopen("./libb.so", RTLD_NOW);
  if (libHandleB == NULL)
    fatal("dlopen: %s", dlerror());

  handler(libHandleA, "liba");
  handler(libHandleB, "libb");

  // (void)dlerror();
  // *(void **)(&funcp) = dlsym(libHandleA, "liba");
  // err = dlerror();
  // if (err != NULL)
  //   fatal("dlsym:%s", err);
  //
  // if (funcp == NULL)
  //   printf("%s is NULL\n", "liba");
  // else
  //   (*funcp)();

  dlclose(libHandleA);

  handler(libHandleA, "liba");

  exit(EXIT_SUCCESS);
}
