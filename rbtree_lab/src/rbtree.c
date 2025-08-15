#include "rbtree.h"

#include <stdlib.h>

/*
  RB 트리 5가지 특성
  1. 모든 노드는 red or black
  2. 루트 노드는 black
  3. 모든 nil 노드는 black
  4. 노드가 red라면 자녀들은 black
  5. 각 노드에서 자손 nil 노드들까지 가는 모든 경로는 black 수와 같다
*/

rbtree *new_rbtree(void) 
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));  

  // 동적할당 NULL 체크
  if (p == NULL)
  {
    return NULL;
  }

  // nil 노드 생성
  p->nil = (node_t *)calloc(1, sizeof(node_t));

  if (p->nil == NULL)
  {
    return NULL;
  }
  
  // color 지정
  p->nil->color = RBTREE_BLACK;
  
  // NULL을 대체하기 위한 노드
  p->nil->left = p->nil->right = p->nil->parent = p->nil;

  // 초기 루트는 nil을 가리킴
  p->root = p->nil;

  return p;
}

void delete_subtree(rbtree* t, node_t* p)
{
  // 베이스 케이스
  if (p == NULL || t->nil == p)
  {
    return;
  }

  delete_subtree(t, p->left);
  delete_subtree(t, p->right);
  free(p);
}

void delete_rbtree(rbtree *t) 
{
  if (t == NULL)
  {
    return;
  }

  delete_subtree(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
