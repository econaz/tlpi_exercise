
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FALSE, TRUE } Boolean;

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
      ptr = tree;
      tree = tree->right;
      if (tree == NULL)
        ptr->right = node;
    } else if (cp < 0) {
      ptr = tree;
      tree = tree->right;
      if (tree == NULL)
        ptr->right = node;
    } else {
      s = pthread_mutex_unlock(&tree->mutex);
      if (s != 0)
        return -1;
      return 0;
    }
    s = pthread_mutex_unlock(&tree->mutex);
    if (s != 0)
      return -1;
  }
  return 0;
}

Tree *lookupTree(Tree *tree, char *key, void **value) {

  if (tree == NULL)
    return NULL;

  int cmp = strcmp(tree->key, key);
  if (cmp > 0)
    lookupTree(tree->right, key, value);
  else if (cmp < 0)
    lookupTree(tree->left, key, value);
  else
    return tree;
  return NULL;
}

int delete(Tree *tree, char *key) {

  Tree *node = lookupTree(tree, key, NULL);
  if (node == NULL)
    return -1;

  int s = pthread_mutex_lock(&tree->mutex);
  if (s != 0)
    return -1;
}

Boolean lookup(Tree *tree, char *key, void **value) {

  if (tree == NULL)
    return FALSE;

  int cmp = strcmp(tree->key, key);
  if (cmp > 0)
    lookup(tree->right, key, value);
  else if (cmp < 0)
    lookup(tree->left, key, value);
  else
    return TRUE;
  return FALSE;
}
