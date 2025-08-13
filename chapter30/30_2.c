
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree Tree;
struct tree {

  char *key;
  void *value;
  Tree *left;
  Tree *right;
  pthread_mutex_t mutex;
};

int initNode(Tree *tree) {
  tree = (Tree *)malloc(sizeof(Tree));
  if (tree == 0)
    return -1;

  int s = pthread_mutex_init(&tree->mutex, NULL);
  if (s != 0)
    return -1;

  tree->key = NULL;
  tree->value = NULL;
  tree->left = tree->right = NULL;
  return 0;
}

int initialize(Tree *tree) {

  initNode(tree);

  // tree = (Tree *)malloc(sizeof(Tree));
  // if (tree == 0)
  //   return -1;
  //
  // int s = pthread_mutex_init(&tree->mutex, NULL);
  // if (s != 0)
  //   return -1;
  //
  // tree->key = NULL;
  // tree->value = NULL;
  // tree->left = tree->right = NULL;
  // return 0;
}

int add(Tree *tree, char *key, void *value) {

  if (key == NULL || value == NULL)
    return -1;

  int s = pthread_mutex_lock(&tree->mutex);
  if (s != 0)
    return -1;

  if (tree->key == NULL) {
    tree->key = key;
    tree->value = value;
    s = pthread_mutex_unlock(&tree->mutex);
    if (s != 0)
      return -1;
    return 0;
  }

  s = pthread_mutex_unlock(&tree->mutex);
  if (s != 0)
    return -1;

  Tree *node = (Tree *)malloc(sizeof(Tree));
  if (node == NULL)
    return -1;
  node->key = key;
  node->value = value;
  node->left = node->right = NULL;
  s = pthread_mutex_init(&node->mutex, NULL);
  if (s != 0)
    return -1;

  while (tree) {

    s = pthread_mutex_lock(&tree->mutex);
    if (s != 0)
      return -1;

    int cp = strcmp(tree->key, key);
    Tree *ptr;
    if (cp > 0) {
      ptr = tree->right;
      if (ptr == NULL) {
        tree->right = node;
        break;
      }
      tree = ptr;
    } else if (cp < 0) {
      ptr = tree->left;
      if (ptr == NULL) {
        tree->left = node;
        break;
      }
      tree = ptr;
    } else {
      s = pthread_mutex_unlock(&tree->mutex);
      // if (s != 0)
      // return -1;
      break;
    }
    s = pthread_mutex_unlock(&tree->mutex);
    if (s != 0)
      return -1;
  }
  return 0;
}

int delete(Tree *tree, char *key) {}

int lookup(Tree *tree, char *key, void **value) {}
