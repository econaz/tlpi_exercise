

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct block {
  int free;
  size_t size;
  struct block *next;
};

typedef struct block bk;
bk *global = NULL;

void *my_malloc(size_t size) {

  bk *ptr = global;
  bk *pre = NULL;
  bk *temp = NULL;
  while (ptr && !(ptr->free && ptr->size >= size)) {
    pre = ptr;
    ptr = ptr->next;
  }

  if (ptr == NULL) {
    ptr = (bk *)sbrk(0);
    if (sbrk(sizeof(bk) + size) == (void *)-1)
      return NULL;

    ptr->size = size;
    ptr->next = NULL;
    ptr->free = 0;

    if (pre) {
      pre->next = ptr;
    } else {
      global = ptr;
    }
    return ptr + sizeof(bk);
  }

  if (ptr->size - size >= sizeof(bk)) {
    temp = ptr;
    temp->size = ptr->size - size - sizeof(bk);
    temp->next = ptr->next;
    temp->free = 1;
    ptr->size = size;
    ptr->next = temp;
    return ptr + sizeof(bk);
  } else {
    temp = (bk *)sbrk(0);

    if (sbrk(sizeof(bk) + size) == (void *)-1)
      return NULL;

    temp->size = size;
    temp->next = ptr;
    pre->next = temp;
    return ptr + sizeof(bk);
  }
  return NULL;
}
int main(int argc, char *argv[]) {
  printf("%10p\n", sbrk(0));
  printf("%lu\n", sizeof(bk));
  sbrk(1999);
  printf("%10p\n", sbrk(0));
  for (int i = 0; i < 10; i++) {
    char *ptr = (char *)my_malloc(1);
    printf("%10p\n", sbrk(0));
  }
  printf("%10p\n", sbrk(0));
}
