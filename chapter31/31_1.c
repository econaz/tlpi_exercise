#include <pthread.h>

typedef enum { TURE, FALSE } Boolean;

typedef void (*func)(void);

typedef struct control control;
struct control {

  Boolean bool;
  pthread_mutex_t mutex;
};

#define PTHREAD_ONCE_INIT1 {PTHREAD_MUTEX_INITIALIZER, FALSE}

control once = PTHREAD_ONCE_INIT1;

int one_time_init(control *control, func init) {

  int s = pthread_mutex_lock(&control->mutex);
  if (s != 0)
    return -1;

  if (control->bool == FALSE) {
    init();
    control->bool = TURE;
  }

  s = pthread_mutex_unlock(&control->mutex);
  if (s != 0)
    return -1;

  return 0;
}
