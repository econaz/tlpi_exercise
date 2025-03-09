

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
bk *split(bk *pre, bk *ptr, size_t size) {
  bk *temp;
  long inside;
  long outsize;
  ptr = (bk *)sbrk(0);
  if (size < sizeof(bk))
    inside = 4 * sizeof(bk);
  else
    inside = 2 * (sizeof(bk) + size);
  if (sbrk(inside) == (void *)-1)
    return NULL;

  ptr->size = size;
  ptr->free = 0;

  outsize = size + sizeof(bk);
  temp = (bk *)((char *)ptr + outsize);
  temp->size = inside - sizeof(bk) - size;
  temp->free = 1;
  temp->next = NULL;

  ptr->next = temp;
  if (pre) {
    pre->next = ptr;
  } else {
    global = ptr;
  }
  return ptr;
}
void *my_malloc(size_t size) {

  bk *ptr = global;
  bk *pre = NULL;
  bk *temp = NULL;
  while (ptr && !(ptr->free && ptr->size >= size)) {
    pre = ptr;
    ptr = ptr->next;
  }
  if (ptr == NULL) {
    printf("ener ptr = null\n");
    ptr = split(pre, ptr, size);
    if (ptr == NULL)
      return NULL;

    return ptr + 1;
  }

  if (ptr->size - size >= sizeof(bk)) {
    if (ptr->size - size == sizeof(bk)) {
      printf("enter size == sizeof(bk)\n");
      ptr->free = 0;
      // ptr->next = ptr->next;
      ptr->size = size;
    } else {
      printf("enter size > sizeof(bk)\n");

      long free_size = ptr->size - size - sizeof(bk);
      if (free_size < sizeof(bk)) {
        temp = NULL;
        if ((void *)-1 == sbrk(-free_size))
          return NULL;

      } else {
        temp = (bk *)((char *)ptr + size + sizeof(bk));
        temp->free = 1;
        temp->next = ptr->next;
        temp->size = free_size;
      }
      ptr->free = 0;
      ptr->size = size;
      ptr->next = temp;
    }
    return ptr + 1;
  } else {
    printf("enter size < sizeof\n");
    ptr = split(pre, ptr, size);
    if (ptr == NULL)
      return NULL;
    return ptr + 1;
  }
  return NULL;
}
void my_free(void *ptr) {

  bk *temp = (bk *)ptr - 1;

  if (temp != NULL && temp->free == 0) {
    temp->free = 1;
  }
}

int main(int argc, char *argv[]) {
  printf("%10p\n", sbrk(0));
  char *ptr = (char *)my_malloc(200);

  for (int i = 0; i < 10; i++) {
    ptr[i] = 'c';
  }
  my_free(ptr);
  for (int i = 0; i < 5; i++) {
    char *ptr1 = (char *)my_malloc(50);
    printf("%10p\n", sbrk(0));
  }

  for (int i = 0; i < 6; i++) {
    ptr = (char *)my_malloc(1);
    printf("%10p\n", sbrk(0));
    if (i == 5)
      printf(" i == 10 %10p\n", ptr);
  }
}
