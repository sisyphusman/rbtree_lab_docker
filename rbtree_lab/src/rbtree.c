#include "rbtree.h"

#include <stdlib.h>

/*
  RB 트리 5가지 특성
  1. 모든 노드는 red or black
  2. 루트 노드는 black
  3. 모든 nil 노드는 black
  4. 노드가 red라면 자녀들은 black
  5. 각 노드에서 자손 nil 노드들까지 가는 모든 경로는 black 수와 같다

 z = new, y = par, x = cur
*/

void delete_subtree(rbtree* t, node_t* p);
void delete_rbtree(rbtree *t);
void left_rotate(rbtree* t, node_t* x);
void right_rotate(rbtree* t, node_t* x);
void insert_fixup(rbtree* t, node_t* new);

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
    free(p);
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
  if (t->nil == p)
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

void left_rotate(rbtree* t, node_t* x)
{
  node_t* y = x->right;
  x->right = y->left;                     // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 회전한다
  
  if (y->left != t->nil)                  // y의 왼쪽 서브 트리가 비어 있지 않은 경우
  {
    y->left->parent = x;                  // 그러면 x는 서브 트리 루트의 부모가 된다
  }

  y->parent = x->parent;                  // x의 부모가 y의 부모가 된다

  if (x->parent == t->nil)                // x가 루트인 경우
  {
    t->root = y;
  }
  else if (x == x->parent->left)          // 그렇지 않으면, x가 왼쪽 자식인 경우
  {
    x->parent->left = y;                  // 그러면 y는 왼쪽 자식이 된다
  }
  else
  {
    x->parent->right = y;
  }
  
  y->left = x;                            // x를 y의 왼쪽 자식이 되게 한다
  x->parent = y;
}

void right_rotate(rbtree* t, node_t* y)
{
  node_t* x = y->left;                    // x는 y의 왼쪽 자식
  y->left = x->right;                     // x의 오른쪽 서브트리를 y의 왼쪽으로 이동

  if (x->right != t->nil)                 // 비어있지 않으면 부모 갱신
  {
    x->right->parent = y;
  }

  x->parent = y->parent;                  // y의 부모를 x의 부모로 교체
  if (y->parent == t->nil)                // y가 루트였다면
  {
    t->root = x;                          
  }
  else if (y == y->parent->right)         // y가 오른쪽 자식이었다면
  {
    y->parent->right = x;                 
  }
  else                                    // y가 왼쪽 자식이었다면
  {
    y->parent->left = x;            
  }

  x->right = y;                           // y를 x의 오른쪽 자식으로
  y->parent = x;
}

void insert_fixup(rbtree* t, node_t* new)
{
  node_t* uncle = t->nil;
  while (new->parent->color == RBTREE_RED)                     // 부모가 RED일 때만 위반이 발생
  {
    if (new->parent == new->parent->parent->left)
    {
      uncle = new->parent->parent->right;

      if (uncle->color == RBTREE_RED)                          // 케이스 1 - new의 부모와 삼촌이 모두 레드인가?
      {
        new->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }
      else
      {
        if (new == new->parent->right)
        {
          new = new->parent;
          left_rotate(t, new);
        }
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        right_rotate(t, new->parent->parent);
      }
    }
    else                                                      //위와 같지만, 오른쪽과 왼쪽이 바뀜
    {
      uncle = new->parent->parent->left;
      if (uncle->color == RBTREE_RED)
      {
        new->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }
      else
      {
        if (new == new->parent->left)
        {
          new = new->parent;
          right_rotate(t, new);
        }
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        left_rotate(t, new->parent->parent);
      }
    }
  }
  
  t->root->color = RBTREE_BLACK;
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
  // 노드 동적 할당
  node_t* new = (node_t*)malloc(sizeof(node_t)); 

  if (new == NULL)
  {
    return NULL;
  }

  // 구조체 설정
  new->color = RBTREE_RED;
  new->parent = new->left = new->right = t->nil;
  new->key = key;

  // 노드 위치 찾기
  node_t* cur = t->root;
  node_t* par = t->nil;

  // 대소 비교
  while (cur != t->nil)
  {
    par = cur;
    if (new->key < cur->key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  new->parent = par;
  if (par == t->nil)
  {
    t->root = new;              // 트리 T가 비어있다
  }
  else if (new->key < par->key)
  {
    par->left = new;
  }
  else
  {
    par->right = new;
  }
  
  insert_fixup(t, new);

  return new;
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
