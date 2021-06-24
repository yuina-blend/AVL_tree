#include <stdio.h>
#include <stdlib.h>

// #include "AVL_tree.h"

typedef struct AVL_tree_node
{
  int value;
  struct AVL_tree_node *right;
  struct AVL_tree_node *left;
} avl_node;

int main(void)
{
  avl_node *root = NULL;

  // 1. 60,40,30,10,50,20の順で挿入
  //  段階的に探索木を出力
  printf("1. 追加\n\n");
  int add_nums[6] = {60, 40, 30, 10, 50, 20};
  for (int i = 0; i < 6; i++)
  {
    root = add_node(root, add_nums[i]);
    print_t(root, 0);
    printf("\n-------------\n");
  }
  // 2. 30と55を検索
  printf("\n\n2. 探索\n\n");
  int search_num[2] = {30, 55};
  for (int i = 0; i < 2; i++)
  {
    avl_node *result = search_node(root, search_num[i]);
    if (result == NULL)
    {
      printf("%dは見つかりませんでした\n", search_num[i]);
    }
    else
    {
      printf("%dを発見しました\n", search_num[i]);
    }
  }

  // 3. 20,40,60,30,50,10の順で削除
  //  段階的に探索木を出力
  printf("\n\n3. 削除\n\n");
  int delete_nums[6] = {20, 40, 60, 30, 50, 10};
  for (int i = 0; i < 6; i++)
  {
    root = delete_node(root, delete_nums[i]);
    print_t(root, 0);
    printf("\n-------------\n");
  }

  deleteTree(root);

  return 0;
}