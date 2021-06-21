ただの二分木
+ 60
---/---/+ 50
---/+ 40
---/---/+ 30
---/---/---/---/+ 20
---/---/---/+ 10

```c
#include <stdio.h>
#include <stdlib.h>
#include "AVL_tree.h"

//新しいノードを作成
struct ALV_tree_node *malloc_node(int value) {
    struct ALV_tree_node *new_node = (struct ALV_tree_node*)malloc(sizeof(struct ALV_tree_node));

    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

//新しいノードを追加
struct ALV_tree_node *add_node(struct ALV_tree_node *root, int value) {
    struct ALV_tree_node *tmp_node;

    if (root == NULL) {
        root = malloc_node(value);
        return root;
    }

    tmp_node = root;
    while (1) {
        if (value < tmp_node->value) {
            if (tmp_node->left == NULL) {
                tmp_node->left = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->left;
        }
        else if (value > tmp_node->value) {
            if (tmp_node->right == NULL) {
                tmp_node->right = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->right;
        }
        else {
            printf("既に存在する値です\n");
            break;
        }
    }
        return root;
}

```

```c
#include <stdio.h>
#include <stdlib.h>
#include "AVL_tree.h"

void printTree(struct ALV_tree_node *root, int depth){
  int i;

  if(root == NULL){
    return ;
  }

  /* 右の子孫ノードを表示 */
  printTree(root->right, depth+1);
 
  /* 深さをスペースで表現 */ 
  for(i = 0; i < depth; i++){
    printf("---/", i);
  }

  /* ノードのデータを表示 */
  printf("+%3d\n", root->value);

  /* 左の子孫ノードを表示 */
  printTree(root->left, depth+1);

  depth++;
}

struct ALV_tree_node *malloc_node(int);
struct ALV_tree_node *add_node(struct ALV_tree_node *, int);

int main() {
    struct ALV_tree_node *root = NULL;
    root = add_node(root, 60);
    root = add_node(root, 40);
    root = add_node(root, 30);
    root = add_node(root, 10);
    root = add_node(root, 50);
    root = add_node(root, 20);
    // printf("%d ",root->);
    printTree(root, 0);
}
```