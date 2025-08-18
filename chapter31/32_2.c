

#include "tlpi_hdr.h"
#include <error_functions.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t key;

static void destructor(void *buf) { free(buf); }

static void createKey(void) {
  int s = pthread_key_create(&key, destructor);
  if (s != 0)
    errExitEN(s, "pthread_key_create");
}

char *dirname_local(char *path) {

  int s, i;
  char *buf;

  s = pthread_once(&once, createKey);
  if (s != 0)
    errExitEN(s, "pthread_once");

  buf = (char *)pthread_getspecific(key);
  if (buf == NULL) {
    buf = (char *)malloc(sizeof(char) * BUF_SIZE);
    if (buf == NULL)
      return NULL;
    pthread_setspecific(key, buf);
  }

  strncpy(buf, path, BUF_SIZE);

  if (buf != NULL && strlen(buf) != 0) {

    i = strlen(buf) - 1;
    if (buf[i] == '/')
      buf[i] = '\0';

    for (; i >= 0; i--) {

      if (buf[i] == '/') {
        if (i == 0) {
          strcpy(buf, "/");
          return buf;
        }
        buf[i] = '\0';
        return buf;
      }
    }
  }
  strcpy(buf, ".");
  return buf;
}

char *basename_local(char *path) {

  int s, i, j = 0, flag = 0;
  char *buf;

  s = pthread_once(&once, createKey);
  if (s != 0)
    errExitEN(s, "pthread_once");

  buf = (char *)pthread_getspecific(key);
  if (buf == NULL) {
    buf = (char *)malloc(sizeof(char) * BUF_SIZE);
    if (buf == NULL)
      return NULL;
    pthread_setspecific(key, buf);
  }
  strncpy(buf, path, BUF_SIZE);

  if (buf != NULL && strlen(buf) != 0) {

    i = strlen(buf) - 1;
    if (i == 0 && buf[i] == '/')
      return buf;
    if (buf[i] == '/')
      buf[i] = '\0';

    for (i = 0; i < strlen(buf); i++) {
      if (flag == 1) {
        buf[j++] = buf[i];
      }
      if (buf[i] == '/') {
        j = 0;
        flag = 1;
      }
    }
    if (flag == 1)
      buf[j] = '\0';
    return buf;
  }

  strcpy(buf, ".");
  return buf;
}

static __thread char buf[BUF_SIZE];

char *dirname_thread(char *path) {
  int i;

  strncpy(buf, path, BUF_SIZE);

  if (buf != NULL && strlen(buf) != 0) {

    i = strlen(buf) - 1;
    if (buf[i] == '/')
      buf[i] = '\0';

    for (; i >= 0; i--) {

      if (buf[i] == '/') {
        if (i == 0) {
          strcpy(buf, "/");
          return buf;
        }
        buf[i] = '\0';
        return buf;
      }
    }
  }
  strcpy(buf, ".");
  return buf;
}
char *basename_thread(char *path) {
  int i, flag = 0, j = 0;

  strncpy(buf, path, BUF_SIZE);

  if (buf != NULL && strlen(buf) != 0) {

    i = strlen(buf) - 1;
    if (i == 0 && buf[i] == '/')
      return buf;
    if (buf[i] == '/')
      buf[i] = '\0';

    for (i = 0; i < strlen(buf); i++) {
      if (flag == 1) {
        buf[j++] = buf[i];
      }
      if (buf[i] == '/') {
        j = 0;
        flag = 1;
      }
    }
    if (flag == 1)
      buf[j] = '\0';
    return buf;
  }
  strcpy(buf, ".");
  return buf;
}
int main(int argc, char *argv[]) {

  char *buf1;
  buf1 = basename_thread("/usr/test");
  printf("%s\n", buf1);
}
