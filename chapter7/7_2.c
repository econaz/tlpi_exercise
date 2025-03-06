

// #include <stddef.h>
// #include <unistd.h>
//
// struct block {
//   _Bool free;
//   size_t size;
//   struct block *next;
// };
//
// typedef struct block bk;
// bk *global = NULL;
//
// void *my_malloc(size_t size) {
//
//   bk *ptr = global;
//   if (ptr) {
//
//   } else {
//     bk *block = sbrk(0);
//     bk *next = sbrk(sizeof(bk) + size);
//     if (next == -1) {
//       return NULL;
//     }
//     block->size = size;
//     block->next = NULL;
//     block->free = TRUE;
//   }
// }
