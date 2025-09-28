#define _GNU_SOURCE
#include "tlpi_hdr.h"
#include <dlfcn.h>

int main(int argc, char *argv[]) {

  void *libHandle;
  void (*funcp)(void);
  const char *err;
  Dl_info info;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s lib-path func-name\n", argv[0]);

  libHandle = dlopen(argv[1], RTLD_LAZY);
  if (libHandle == NULL)
    fatal("dlopen:%s", dlerror());

  (void)dlerror();
  // *(void **)(&funcp) = dlsym(libHandle, argv[2]);
  err = dlerror();
  if (err != NULL)
    fatal("dlsym:%s", err);

  dladdr(libHandle, &info);

  printf("pathname = %s, base addr = %s,symbol name = %s,symbol value = %s",
         info.dli_fname, (char *)info.dli_fbase, (char *)info.dli_sname,
         (char *)info.dli_saddr);

  if (funcp == NULL)
    printf("%s is NULL\n", argv[2]);
  else
    (*funcp)();

  dlclose(libHandle);
  exit(EXIT_SUCCESS);
}
