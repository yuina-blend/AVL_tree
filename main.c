#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

void printTree(struct binary_tree_node *root, int depth){
  int i;

  if(root == NULL){
    return ;
  }

  /* 右の子孫ノードを表示 */
  printTree(root->right, depth+1);
 
  /* 深さをスペースで表現 */ 
  for(i = 0; i < depth; i++){
    printf("  ");
  }

  /* ノードのデータを表示 */
  printf("+%3d\n", root->value);

  /* 左の子孫ノードを表示 */
  printTree(root->left, depth+1);

  depth++;
}

struct binary_tree_node *malloc_node(int);
struct binary_tree_node *add_node(struct binary_tree_node *, int);

int main() {
    struct binary_tree_node *root = NULL;
    root = add_node(root, 60);
    root = add_node(root, 40);
    root = add_node(root, 30);
    root = add_node(root, 10);
    root = add_node(root, 50);
    root = add_node(root, 20);
    // printf("%d ",root->);
    printTree(root, 0);
}