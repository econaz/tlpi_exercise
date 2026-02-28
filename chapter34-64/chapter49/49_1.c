

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int fd, srcfd;
  struct stat st;
  char *dst, *src;
  size_t size;

  if (argc < 2 || strcmp(argv[1], "--help") == 1)
    usageErr("usage %s filename desfilname", argv[0]);

  fd = open(argv[1], O_RDWR);
  
  if (fd == -1)
    errExit("open");

  if (fstat(fd, &st) == -1)
    errExit("fstat");

  if (st.st_size == 0)
    errExit("file size is 0");

  dst = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (dst == MAP_FAILED)
    errExit("mmap");

  srcfd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR,S_IRUSR | S_IRGRP | S_IROTH);
  if (srcfd == -1)
    errExit("open");

  if (ftruncate(srcfd, st.st_size) == -1)
    errExit("ftruncate");

  src = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, srcfd, 0);
  if (src == MAP_FAILED)
    errExit("mmap");

  memcpy(src, dst, st.st_size);

  if (close(fd) == -1)
    errExit("close");

  if (close(srcfd) == -1)
    errExit("close");

  if (msync(src, st.st_size, MS_ASYNC) == -1)
    errExit("msync");

  if (munmap(src, st.st_size) == -1)
    errExit("munmap");

  if (munmap(dst, st.st_size) == -1)
    errExit("munmap");

  exit(EXIT_SUCCESS);
}
