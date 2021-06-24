# データ構造とアルゴリズムⅡ AVL木レポート

報告者: 4I 35番 弘中悠月

提出日: 2021/06/21(月)



## 設計

### 木の構造

```	c : AVL_tree.h
typedef struct AVL_tree_node
{
    int value;
    struct AVL_tree_node *left;
    struct AVL_tree_node *right;
} avl_node;
```

構造体を用いたリスト構造でAVL木を実装する。`int value`はデータの値を格納、`struct AVL_tree_node *left, struct AVL_tree_node *right`はそれぞれ左と右へのポインタを保持し、これらを連結することで木構造を表現する。



### 実装の方針

まず単純な二分探索木の追加機能を実装し、木構造が正しく機能することを確認する。その後追加機能の改造と探索、削除機能を実装する。

プログラムファイルの分割は`AVL_tree.h`に木構造の定義、`AVL_tree.c`にAVL木の機能の定義、`main.c`に今回の課題で行う操作を記述する。

ただし、今回はコンパイル時の作業簡略化のため、`DSaA_AVL.c`の一つのファイルに記述した。本レポートではこのプログラムについて記述していく。



### 二分木 追加機能/木構造の確認

まず、単純な二分探索木の追加機能のみを実装し、木構造が正しく動作しているかを確かめた。以下のコードの`malloc_node, add_node`が追加機能に当たる関数である。木の表示には`printTree`という関数を定義し使用しているが、これは後のAVL木の実装段階で修正したため解説は省略する。

```c : binary_tree.c
#include <stdio.h>
#include <stdlib.h>
#include "AVL_tree.h"

//新しいノードを作成
struct ALV_tree_node *malloc_node(int value)
{
    struct ALV_tree_node *new_node = (struct ALV_tree_node *)malloc(sizeof(struct ALV_tree_node));

    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

//新しいノードを追加
struct ALV_tree_node *add_node(struct ALV_tree_node *root, int value)
{
    struct ALV_tree_node *tmp_node;

    if (root == NULL)
    {
        root = malloc_node(value);
        return root;
    }

    tmp_node = root;
    while (1)
    {
        if (value < tmp_node->value)
        {
            if (tmp_node->left == NULL)
            {
                tmp_node->left = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->left;
        }
        else if (value > tmp_node->value)
        {
            if (tmp_node->right == NULL)
            {
                tmp_node->right = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->right;
        }
        else
        {
            printf("既に存在する値です\n");
            break;
        }
    }
    return root;
}

void printTree(struct ALV_tree_node *root, int depth)
{
    int i;

    if (root == NULL)
    {
        return;
    }

    /* 右の子孫ノードを表示 */
    printTree(root->right, depth + 1);

    /* 深さをスペースで表現 */
    for (i = 0; i < depth; i++)
    {
        printf("---/", i);
    }

    /* ノードのデータを表示 */
    printf("+%3d\n", root->value);

    /* 左の子孫ノードを表示 */
    printTree(root->left, depth + 1);

    depth++;
}

struct ALV_tree_node *malloc_node(int);
struct ALV_tree_node *add_node(struct ALV_tree_node *, int);

int main()
{
    struct ALV_tree_node *root = NULL;
    root = add_node(root, 60);
    root = add_node(root, 40);
    root = add_node(root, 30);
    root = add_node(root, 10);
    root = add_node(root, 50);
    root = add_node(root, 20);

    printTree(root, 0);
}
```

以下の出力がこのコードの実行結果である。

```
+ 60
---/---/+ 50
---/+ 40
---/---/+ 30
---/---/---/---/+ 20
---/---/---/+ 10
```

<img src="/home/blend/.config/Typora/typora-user-images/image-20210621202335800.png" alt="image-20210621202335800" style="zoom:25%;" />

図で表すと上図のような出力になっていることがわかり、二分木の構造については問題ないことがわかった。



### AVL木 回転/追加

AVL木においては平衡係数の絶対値が1以下である必要があるため、二分木の実装に加えて平衡係数の修正を行う**回転**の動作が必要である。

回転動作は4種類実装する必要があり、

1.  平衡係数が-1より小さい場合は部分木を左に回転させる
2.  平衡係数が1より大きい場合は部分木を右に回転させる
3.  平衡係数が-1より小さくてかつ、左の子ノードの平衡係数が1以上の場合は、左の子ノードを根として左回転、元のノードを根として右回転させる
4.  平衡係数が1より大きくてかつ、右の子ノードの平衡係数が-1以下の場合は、右の子ノードを根として右回転、元のノードを根として左回転させる

上記4つの回転が必要である。

以下に示す図は回転(1枚目)、追加(2枚目)機能のフローチャートである。

![image-20210621210523514](/home/blend/.config/Typora/typora-user-images/image-20210621210523514.png)

![image-20210621211038173](/home/blend/.config/Typora/typora-user-images/image-20210621211038173.png)



### AVL木 探索

探索な二分木と同様であり、探索値がノードの値より大きければ右へ、小さければ左へ進めば良い。以下の図は探索機能のフローチャートである。

![image-20210621212730285](/home/blend/.config/Typora/typora-user-images/image-20210621212730285.png)



### AVL木 削除

削除は3つの場合に分けて実装する。具体的には、

1.  葉の場合は、そのまま削除する
2.  子を1つ持つ場合、部分木を昇格させる
3.  子を2つ持つ場合、左部分木の最大ノードを昇格させる

上記の3つの機能が必要である。以下の図は削除機能のフローチャートである。

![image-20210621214907202](/home/blend/.config/Typora/typora-user-images/image-20210621214907202.png)



## 実装

### コード

前述の通り、プログラムは`DSaA_AVL.c`に結合して作成した。以下のコードがその内容である。

```c : DSaA_AVL.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEIGHT 100

#define TREE_LEFT 1
#define TREE_RIGHT 2

typedef struct AVL_tree_node
{
    int value;
    struct AVL_tree_node *right;
    struct AVL_tree_node *left;
} avl_node;

int get_height(avl_node *node)
{
    if (node == NULL)
    {
        return 0;
    }

    int left_height = get_height(node->left);
    int right_height = get_height(node->right);

    if (left_height > right_height)
    {
        return left_height + 1;
    }
    else
    {
        return right_height + 1;
    }
}

avl_node *left_rotate(avl_node *root, avl_node *node, avl_node *parent, int direction)
{
    avl_node *pivot;
    avl_node *new_root;

    printf("left_rotate:%d\n", node->value);

    pivot = node->right;

    if (pivot != NULL)
    {
        node->right = pivot->left;
        pivot->left = node;
    }

    if (parent == NULL)
    {
        new_root = pivot;
        return new_root;
    }

    if (direction == TREE_LEFT)
    {
        parent->left = pivot;
    }
    else
    {
        parent->right = pivot;
    }
    return root;
}

avl_node *right_rotate(avl_node *root, avl_node *node, avl_node *parent, int direction)
{
    avl_node *pivot;
    avl_node *new_root;

    printf("right_rotate:%d\n", node->value);

    pivot = node->left;

    if (pivot != NULL)
    {
        node->left = pivot->right;
        pivot->right = node;
    }

    if (parent == NULL)
    {
        new_root = pivot;
        return new_root;
    }

    if (direction == TREE_LEFT)
    {
        parent->left = pivot;
    }
    else
    {
        parent->right = pivot;
    }
    return root;
}

avl_node *rightleft_rotate(avl_node *root, avl_node *node, avl_node *parent, int direction)
{
    avl_node *new_root;
    printf("right_left_rotate:%d\n", node->value);

    new_root = right_rotate(root, node->right, node, TREE_RIGHT);

    return left_rotate(new_root, node, parent, direction);
}

avl_node *leftright_rotate(avl_node *root, avl_node *node, avl_node *parent, int direction)
{
    avl_node *new_root;

    printf("left_right_rotate:%d\n", node->value);

    new_root = left_rotate(root, node->left, node, TREE_LEFT);

    return right_rotate(new_root, node, parent, direction);
}

avl_node *balancing(avl_node *root, avl_node *node, avl_node *parent, int direction, int *branch, int num_branch)
{
    avl_node *next;
    avl_node *new_root;

    int left_height, right_height;
    int balance;

    if (node == NULL || root == NULL)
    {
        return root;
    }

    if (num_branch > 0)
    {
        if (branch[0] == TREE_LEFT)
        {
            next = node->left;
        }
        else
        {
            next = node->right;
        }
        new_root = balancing(root, next, node, branch[0], &branch[1], num_branch - 1);
    }

    left_height = get_height(node->left);
    right_height = get_height(node->right);
    balance = right_height - left_height;

    if (balance > 1)
    {
        if (get_height(node->right->left) > get_height(node->right->right))
        {
            return rightleft_rotate(new_root, node, parent, direction);
        }
        else
        {
            return left_rotate(new_root, node, parent, direction);
        }
    }
    else if (balance < -1)
    {
        if (get_height(node->left->right) > get_height(node->left->left))
        {
            return leftright_rotate(new_root, node, parent, direction);
        }
        else
        {
            return right_rotate(new_root, node, parent, direction);
        }
    }

    return root;
}

avl_node *malloc_node(int value)
{
    avl_node *new_node;

    new_node = (avl_node *)malloc(sizeof(avl_node));
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

avl_node *add_node(avl_node *root, int value)
{
    avl_node *node;
    int branch[MAX_HEIGHT] = {0};
    int num_branch = 0;

    if (root == NULL)
    {
        root = malloc_node(value);
        if (root == NULL)
        {
            printf("malloc error\n");
            return NULL;
        }
        return root;
    }

    node = root;
    while (1)
    {
        if (value < node->value)
        {
            if (node->left == NULL)
            {
                node->left = malloc_node(value);
                break;
            }
            branch[num_branch] = TREE_LEFT;
            num_branch++;
            node = node->left;
        }
        else if (value > node->value)
        {
            if (node->right == NULL)
            {
                node->right = malloc_node(value);
                break;
            }
            branch[num_branch] = TREE_RIGHT;
            num_branch++;
            node = node->right;
        }
        else
        {
            printf("%dは既に存在します\n", value);
            break;
        }
    }

    return balancing(root, root, NULL, 0, branch, num_branch);
}

avl_node *search_node(avl_node *root, int value)
{
    avl_node *node;
    node = root;
    while (node)
    {
        if (value < node->value)
        {
            node = node->left;
        }
        else if (value > node->value)
        {
            node = node->right;
        }
        else
        {
            return node;
        }
    }

    return NULL;
}

avl_node *delete_not_have_child_node(avl_node *root, avl_node *node, avl_node *parent)
{
    if (parent != NULL)
    {
        if (parent->left == node)
        {
            parent->left = NULL;
        }
        else
        {
            parent->right = NULL;
        }
        free(node);
    }
    else
    {
        free(node);
        root = NULL;
    }
    return root;
}

avl_node *delete_have_one_child_node(avl_node *root, avl_node *node, avl_node *child)
{
    node->value = child->value;
    node->left = child->left;
    node->right = child->right;

    free(child);

    return root;
}

avl_node *delete_have_two_child_node(avl_node *root, avl_node *node, int *branch, int *num_branch)
{
    avl_node *max;
    avl_node *maxParent;

    max = node->left;
    maxParent = node;

    branch[*num_branch] = TREE_LEFT;
    (*num_branch)++;

    while (max->right != NULL)
    {
        maxParent = max;
        max = max->right;

        branch[*num_branch] = TREE_RIGHT;
        (*num_branch)++;
    }
    printf("max value is %d\n", max->value);

    node->value = max->value;

    if (max->left == NULL)
    {
        root = delete_not_have_child_node(root, max, maxParent);
    }
    else
    {
        root = delete_have_one_child_node(root, max, max->left);
    }

    return root;
}

avl_node *delete_node(avl_node *root, int value)
{
    avl_node *node;
    avl_node *parent;
    int branch[MAX_HEIGHT] = {0};
    int num_branch = 0;

    if (root == NULL)
    {
        return NULL;
    }

    node = root;
    parent = NULL;

    while (node != NULL)
    {
        if (value < node->value)
        {
            parent = node;
            node = node->left;

            branch[num_branch] = TREE_LEFT;
            num_branch++;
        }
        else if (value > node->value)
        {
            parent = node;
            node = node->right;

            branch[num_branch] = TREE_RIGHT;
            num_branch++;
        }
        else
        {
            break;
        }
    }

    if (node == NULL)
    {
        printf("%dを持つノードが存在しません\n", value);
        return root;
    }

    printf("Delete %d node\n", node->value);

    if (node->left == NULL && node->right == NULL)
    {
        root = delete_not_have_child_node(root, node, parent);
    }
    else if ((node->left != NULL && node->right == NULL) || (node->right != NULL && node->left == NULL))
    {
        if (node->left != NULL)
        {
            root = delete_have_one_child_node(root, node, node->left);
        }
        else
        {
            root = delete_have_one_child_node(root, node, node->right);
        }
    }
    else
    {
        root = delete_have_two_child_node(root, node, branch, &num_branch);
    }

    return balancing(root, root, NULL, 0, branch, num_branch);
}

void print_t(avl_node *root, int h)
{
    if (root != NULL)
    {
        print_t(root->right, h + 1);
        for (int i = 0; i < h; i++)
        {
            printf("|---|");
        }
        printf("%d\n", root->value);
        print_t(root->left, h + 1);
    }
}

void deleteTree(avl_node *root)
{
    if (root == NULL)
    {
        return;
    }

    if (root->left != NULL)
    {
        deleteTree(root->left);
    }
    if (root->right != NULL)
    {
        deleteTree(root->right);
    }

    free(root);
}

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
```



### 実行結果

#### 実行内容

1. 60,40,30,10,50,20の順で挿入
段階的に探索木を出力
2. 30と55を検索
3. 20,40,60,30,50,10の順で削除
段階的に探索木を出力

実行内容は`main関数`に記載している。以下に示す出力が`DSaA_AVL.c`の実行結果である。

```
1. 追加

60

-------------
60
|---|40

-------------
right_rotate:60
|---|60
40
|---|30

-------------
|---|60
40
|---|30
|---||---|10

-------------
|---|60
|---||---|50
40
|---|30
|---||---|10

-------------
left_right_rotate:30
left_rotate:10
right_rotate:30
|---|60
|---||---|50
40
|---||---|30
|---|20
|---||---|10

-------------


2. 探索

30を発見しました
55は見つかりませんでした


3. 削除

Delete 20 node
max value is 10
|---|60
|---||---|50
40
|---||---|30
|---|10

-------------
Delete 40 node
max value is 30
|---|60
|---||---|50
30
|---|10

-------------
Delete 60 node
|---|50
30
|---|10

-------------
Delete 30 node
max value is 10
|---|50
10

-------------
Delete 50 node
10

-------------
Delete 10 node

-------------
```

出力の`|---|`は木の高さを示しており、また、高さ0のノードを基準として下の値が左、上の値が右というように対応している。



### 参考文献

コードの設計・作成にあたり以下の文献を参照した。

-   https://daeudaeu.com/bintree/
-   https://daeudaeu.com/avl_tree/#i-3

-   http://www.nct9.ne.jp/m_hiroi/linux/clang13.html

以下に参照したコードを示す。



#### 単純二分探索木

```c : https://daeudaeu.com/bintree/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 256

/* 二分探索木のノードを表す構造体 */
struct node_t {
  int number;
  char name[MAX_NAME_LEN];
  struct node_t *left;
  struct node_t *right;
};


/* deleteTree:二分探索木のノード全てを削除する
   引数１ root : 根ノードのアドレス
   返却値 : なし */
void deleteTree(struct node_t *root){
  if(root == NULL){
    return;
  }

  deleteTree(root->left);
  deleteTree(root->right);

  printf("free:%d(%s)\n", root->number, root->name);
  free(root);

}

/* mallocNode:ノードの構造体のメモリを確保し、データを設定
   引数１ number : 追加する会員番号
   引数２ name : 追加する会員の名前
   返却値 : 追加したノードのアドレス */
struct node_t *mallocNode(int number, char *name){
  struct node_t *add;

  add = (struct node_t*)malloc(sizeof(struct node_t));
  if(add == NULL){ 
    return NULL;
  }

  add->left = NULL;
  add->right = NULL;
  add->number = number;
  strcpy(add->name, name);

  return add;
}

/* addNode:指定されたnumberとname持つノードを追加する
   引数１ root : 根ノードのアドレス
   引数２ number : 追加する会員番号
   引数３ name : 追加する会員の名前
   返却値 : 根ルートのアドレス */
struct node_t *addNode(struct node_t *root, int number, char *name){
  struct node_t *node;

  /* まだノードが一つもない場合 */
  if(root == NULL){
    /* 根ノードとしてノードを追加 */
    root = mallocNode(number, name);
    if(root == NULL){
      printf("malloc error\n");
      return NULL;
      }
      return root;
   }

   /* 根ノードから順に追加する場所を探索 */
   node = root;
   while(1) {
     if(number < node->number){
       /* 追加する値がノードの値よりも小さい場合 */

       if(node->left == NULL){
         /* そのノードの左の子が無い場合（もう辿るべきノードが無い場合）*/

         /* その左の子の位置にノードを追加 */
         node->left = mallocNode(number, name);

         /* 追加完了したので処理終了 */
         break;
       }

       /* 左の子がある場合は左の子を新たな注目ノードに設定 */
       node = node->left;

     } else if(number > node->number){
       /* 追加する値がノードの値よりも大きい場合 */

       if(node->right == NULL){
         /* そのノードの右の子が無い場合（もう辿るべきノードが無い場合）*/

         /* その右の子の位置にノードを追加 */
         node->right = mallocNode(number, name);

         /* 追加完了したので処理終了 */
         break;
       }

       /* 右の子がある場合は右の子を新たな注目ノードに設定 */
       node = node->right;
     } else {
       /* 追加する値とノードの値が同じ場合 */

       printf("%d already exist\n", number);
       break;
    }
   }
    
   return root;
}

/* searchNode:指定されたnumberを持つノードを探索する
   引数１ root : 探索を開始するノードのアドレス
   引数２ number : 探索する会員番号
   返却値 : number を持つノードのアドレス（存在しない場合は NULL）*/
struct node_t *searchNode(struct node_t *root, int number){
  struct node_t *node;
  
  node = root;

  /* 探索を行うループ（注目ノードがNULLになったら終了 */
  while(node){
    if(number < node->number){
      /* 探索値がノードの値よりも小さい場合 */

      /* 注目ノードを左の子ノードに設定 */
      node = node->left;
    } else if(number > node->number){
      /* 探索値がノードの値よりも大きい場合 */

      /* 注目ノードを右の子ノードに設定 */
      node = node->right;
    } else {
      /* 探索値 = ノードの値の場合 */
      return node;
    }
  }
  
  /* 探索値を持つノードが見つからなかった場合 */
  return NULL;
}

/* deleteNoChildeNode:指定された子の無いノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   引数３ parent：削除するノードの親ノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteNoChildNode(struct node_t *root, struct node_t *node, struct node_t *parent){

  if(parent != NULL){
    /* 親がいる場合（根ノード以外の場合）は
    削除対象ノードを指すポインタをNULLに設定 */
    if(parent->left ==  node){
       /* 削除対象ノードが親ノードから見て左の子の場合 */
        parent->left = NULL;
    } else {
      /* 削除対象ノードが親ノードから見て右の子の場合 */
      parent->right = NULL;
    }
    free(node);
  }  else {
    /* 削除対象ノードが根ノードの場合 */
    free(node);
      
    /* 根ノードを指すポインタをNULLに設定 */
    root = NULL;
  }
    
  return root;
}

/* deleteOneChildeNode:指定された子が一つのノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   引数３ child : 削除するノードの子ノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteOneChildNode(struct node_t *root, struct node_t *node, struct node_t * child){
   
  /* 削除対象ノードにその子ノードのデータとポインタをコピー */
  node->number = child->number;
  strcpy(node->name, child->name);
  node->left = child->left;
  node->right = child->right;
    
  /* コピー元のノードを削除 */
  free(child);
  
  return root;
}

/* deleteTwoChildeNode:指定された子が二つのノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteTwoChildNode(struct node_t *root, struct node_t *node){

  struct node_t *max;
  struct node_t *maxParent;
  
  /* 左の子から一番大きい値を持つノードを探索 */
  max = node->left;
  maxParent = node;
    
  while(max->right != NULL){
    maxParent = max;
    max = max->right;
  }
  printf("max number is %d\n", max->number);
    
  /* 最大ノードのデータのみ削除対象ノードにコピー */
  node->number = max->number;
  strcpy(node->name, max->name);
    
  /* 最大ノードを削除 */
  
  /* maxは最大ノードなので必ずmax->rightはNULLになる */
  if(max->left == NULL){
    /* 最大ノードに子がいない場合 */
    root = deleteNoChildNode(root, max, maxParent);      
    } else {
      /* 最大ノードに子供が一ついる場合 */
      root = deleteOneChildNode(root, max, max->left);
    }

    return root;
}



/* deleteNode:指定されたnumberを持つノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ number : 削除する会員番号
   返却値 : 根ノードのアドレス */
struct node_t *deleteNode(struct node_t *root, int number){
  struct node_t *node;
  struct node_t *parent;

  if(root == NULL){
    return NULL;
  }

  /* 削除対象ノードを指すノードを探索 */
  node = root;
  parent = NULL;
  
  while(node !=  NULL){
    if(number < node->number){
      parent = node;
      node = node->left;
    } else if(number > node->number){
      parent = node;
      node = node->right;
    } else {
      break;
    }
  }
  
  /* 指定されたnumberを値として持つノードが存在しない場合は何もせず終了 */
  if(node == NULL){
    printf("%d を持つノードが存在しません\n", number);
    return root;
  }

  printf("Delete %d(%s) node\n", node->number, node->name);

  if(node->left == NULL && node->right == NULL){    
    /* 子がいないノードの削除 */
    root = deleteNoChildNode(root, node, parent);
  } else if((node->left != NULL && node->right == NULL) ||
    (node->right != NULL && node->left == NULL)){
    /* 子が一つしかない場合 */
    
    if(node->left != NULL){
      root = deleteOneChildNode(root, node, node->left);
    } else {
      root = deleteOneChildNode(root, node, node->right);
    }
  } else {
    /* 左の子と右の子両方がいるノードの削除 */
    root = deleteTwoChildNode(root, node);
  }
  
  return root;  
}

/* printTree:rootを根ノードとする二分探索木をの全ノードを表示する
   引数１ root : 木の根ノードのアドレス
   引数２ depth: 関数呼び出しの深さ
   返却値 : なし */
void printTree(struct node_t *root, int depth){
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
  printf("+%3d(%s)\n", root->number, root->name);

  /* 左の子孫ノードを表示 */
  printTree(root->left, depth+1);

  depth++;
}

int main(void){
  struct node_t *root, *node;
  int input;
  int number;
  char name[MAX_NAME_LEN];
  int loop;

  /* まだ木がないのでrootをNULLにセット */
  root = NULL;

  /* 最初にてきとうにノードを追加しておく */
  root = addNode(root, 100, "100");
  root = addNode(root, 200, "200");
  root = addNode(root, 300, "300");
  root = addNode(root, 50, "50");
  root = addNode(root, 150, "150");
  root = addNode(root, 250, "250");
  root = addNode(root, 10, "1");
  root = addNode(root, 125, "125");
  root = addNode(root, 5, "5");
  root = addNode(root, 25, "25");
  root = addNode(root, 500, "500");
  root = addNode(root, 175, "175");

  loop = 1;
  while(loop){
    printf("処理を選択(1:add, 2:delete, 3:search, 4:exit)");
    scanf("%d", &input);

    switch(input){
    case 1:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }

      printf("名前:");
      scanf("%s", name);

      root = addNode(root, number, name);
      break;
    case 2:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }

      root = deleteNode(root, number);
      
      break;
    case 3:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }
      
      node = searchNode(root, number);
      if(node == NULL){
        printf("number %d is not found\n", number);
      } else {
        printf("number %d : %s\n", number, node->name);
      }
      break;
    default:
      loop = 0;
      break;
    }
    printTree(root, 0);
  }

  deleteTree(root);   
  
  return 0;
}
```



#### AVL木

```c : https://daeudaeu.com/avl_tree/#i-3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 256
#define MAX_HEIGHT 100

#define TREE_LEFT 1
#define TREE_RIGHT 2

/* 二分探索木のノードを表す構造体 */
struct node_t {
  int number;
  char name[MAX_NAME_LEN];
  struct node_t *left;
  struct node_t *right;
};

/* getHeight:二分探索木のノード全てを削除する
   引数１ node : 根ノードのアドレス
   返却値 : nodeを根とした木の高さ */
int getHeight(struct node_t *node) {

  int left_height;
  int right_height;
  int tree_height;

  if (node == NULL) {
    /* nodeが無いなら高さは0 */
    return 0;
  }

  /* 左右の子を根とした木の高さを取得 */
  left_height = getHeight(node->left);
  right_height = getHeight(node->right);

  /* 大きい方に+1したものを木の高さとして返却 */
  if (left_height > right_height) {
    tree_height = left_height;
  } else {
    tree_height = right_height;
  }

  return tree_height + 1;
}

/* leftRotate:nodeを根とする部分木を回転（左）
   引数１ root : 根のノードを指すアドレス
   引数２ node : 回転する部分木の根ノードを指すアドレス
   引数３ parent : nodeの親ノードを指すアドレス
   引数４ direction : parentから見たnodeのある方向
   返却値 : 根のノードを指すアドレス */
struct node_t *leftRotate(struct node_t *root, struct node_t *node, struct node_t *parent, int direction) {
  /* nodeを根として左回転を行う */

  struct node_t *pivot;
  struct node_t *new_root;

  printf("left_rotate:%d\n", node->number);

  /* 新しい根とするノードをpivotとして設定 */
  pivot = node->right;

  /* 左回転 */
  if (pivot != NULL) {
    node->right = pivot->left;
    pivot->left = node;
  }

  /* parentもしくはrootに新しい根ノードを参照させる */
  if (parent == NULL) {
    new_root = pivot;
    return new_root;
  }

  /* どちらの子に設定するかはdirectionから判断 */
  if (direction == TREE_LEFT) {
    parent->left = pivot;
  } else {
    parent->right = pivot;
  }
  return root;
}

/* rightRotate:nodeを根とする部分木を回転（右）
   引数１ root : 根のノードを指すアドレス
   引数２ node : 回転する部分木の根ノードを指すアドレス
   引数３ parent : nodeの親ノードを指すアドレス
   引数４ direction : parentから見たnodeのある方向
   返却値 : 根のノードを指すアドレス */
struct node_t * rightRotate(struct node_t *root, struct node_t *node, struct node_t *parent, int direction) {

  struct node_t *pivot;
  struct node_t *new_root;

  printf("right_rotate:%d\n", node->number);

  /* 新しい根とするノードをpivotとして設定 */
  pivot = node->left;

  /* 右回転 */
  if (pivot != NULL) {
    node->left = pivot->right;
    pivot->right = node;
  }

  /* parentもしくはrootに新しい根ノードを参照させる */
  if (parent == NULL) {
    new_root = pivot;
    return new_root;
  }

  /* どちらの子に設定するかはdirectionから判断 */
  if (direction == TREE_LEFT) {
    parent->left = pivot;
  } else {
    parent->right = pivot;
  }

  return root;
}

/* leftRightRotate:nodeを根とする部分木を二重回転（右->左）
   引数１ root : 根のノードを指すアドレス
   引数２ node : 回転する部分木の根ノードを指すアドレス
   引数３ parent : nodeの親ノードを指すアドレス
   引数４ direction : parentから見たnodeのある方向
   返却値 : 根のノードを指すアドレス */
struct node_t *rightLeftRotate(struct node_t *root, struct node_t *node, struct node_t *parent, int direction) {
  /* ２重回転（Right Left Case）を行う */

  struct node_t *new_root;
  printf("right_left_rotate:%d\n", node->number);

  /* nodeの右の子ノードを根として右回転 */
  new_root = rightRotate(root, node->right, node, TREE_RIGHT);

  /* nodeを根として左回転 */
  return leftRotate(new_root, node, parent, direction);
}

/* leftRightRotate:nodeを根する部分木を二重回転（左->右）
   引数１ root : 根のノードを指すアドレス
   引数２ node : 回転する部分木の根ノードを指すアドレス
   引数３ parent : nodeの親ノードを指すアドレス
   引数４ direction : parentから見たnodeのある方向
   返却値 : 根のノードを指すアドレス */
struct node_t * leftRightRotate(struct node_t *root, struct node_t *node, struct node_t *parent, int direction) {
  /* ２重回転（Left Right Case）を行う */

  struct node_t *new_root;

  printf("left_right_rotate:%d\n", node->number);

  /* nodeの左の子ノードを根として左回転 */
  new_root = leftRotate(root, node->left, node, TREE_LEFT);

  /* nodeを根として右回転 */
  return rightRotate(new_root, node, parent, direction);
}

/* balancing:nodeからbranchで辿ったノードを平衡にする
   引数１ root : 根のノードを指すアドレス
   引数２ node : 平衡にするノードを指すアドレス
   引数３ parent : nodeの親ノードを指すアドレス
   引数４ direction : parentから見たnodeのある方向
   引数５ branch : 平衡化を行うノードへの経路
   引数６ num_branch : branchに格納された経路の数
   返却値 : 根のノードを指すアドレス */
struct node_t * balancing(struct node_t *root, struct node_t *node, struct node_t *parent, int direction, int *branch, int num_branch) {

  struct node_t *next;
  struct node_t *new_root;

  int left_height, right_height;
  int balance;

  if (node == NULL || root == NULL) {
    return root;
  }

  if (num_branch > 0) {
    /* 辿れる場合はまず目的のノードまで辿る */

    /* 辿る子ノードを設定 */
    if (branch[0] == TREE_LEFT) {
      next = node->left;
    } else {
      next = node->right;
    }

    /* 子ノードを辿る */
    new_root = balancing(root, next, node, branch[0], &branch[1], num_branch - 1);
  }
    
  /* 平衡係数を計算 */
  left_height = getHeight(node->left);
  right_height = getHeight(node->right);
  balance = right_height - left_height;
  
  if (balance > 1) {
    /* 右の部分木が高くて並行状態でない場合 */

    /* ２重回転が必要かどうかを判断 */
    if (getHeight(node->right->left) > getHeight(node->right->right)) {
      /* ２重回転（Right Left Case）*/
      return rightLeftRotate(new_root, node, parent, direction);

    } else {
      /*１重回転（左回転）*/
      return leftRotate(new_root, node, parent, direction);
    }
  
  } else if (balance < -1) {
    /* 左の部分木が高くて並行状態でない場合 */

    /* ２重回転が必要かどうかを判断 */
    if (getHeight(node->left->right) > getHeight(node->left->left)) {
      /* ２重回転（Left Right Case）*/
      return leftRightRotate(new_root, node, parent, direction);
    } else {
      /* １重回転（右回転）*/
      return rightRotate(new_root, node, parent, direction);
    }
  }

  return root;
}

/* deleteTree:二分探索木のノード全てを削除する
   引数１ root : 根ノードのアドレス
   返却値 : なし */
void deleteTree(struct node_t *root){
  if(root == NULL){
    return;
  }

  if(root->left != NULL){
    deleteTree(root->left);
  }
  if(root->right != NULL){
    deleteTree(root->right);
  }

  printf("free:%d(%s)\n", root->number, root->name);
  free(root);

}

/* mallocNode:ノードの構造体のメモリを確保し、データを設定
   引数１ number : 追加する会員番号
   引数２ name : 追加する会員の名前
   返却値 : 追加したノードのアドレス */
struct node_t *mallocNode(int number, char *name){
  struct node_t *add;

  add = (struct node_t*)malloc(sizeof(struct node_t));
  if(add == NULL){ 
    return NULL;
  }

  add->left = NULL;
  add->right = NULL;
  add->number = number;
  strcpy(add->name, name);

  return add;
}

/* addNode:指定されたnumberとname持つノードを追加する
   引数１ root : 根ノードのアドレス
   引数２ number : 追加する会員番号
   引数３ name : 追加する会員の名前
   返却値 : 根ルートのアドレス */
struct node_t *addNode(struct node_t *root, int number, char *name){
  struct node_t *node;
  int branch[MAX_HEIGHT] = {0};
  int num_branch = 0;

  /* まだノードが一つもない場合 */
  if(root == NULL){
    /* 根ノードとしてノードを追加 */
    root = mallocNode(number, name);
    if(root == NULL){
      printf("malloc error\n");
      return NULL;
    }
    return root;
  }

  /* 根ノードから順に追加する場所を探索 */
  node = root;
  while(1) {
    if(number < node->number){
      /* 追加する値がノードの値よりも小さい場合 */

      if(node->left == NULL){
        /* そのノードの左の子が無い場合（もう辿るべきノードが無い場合）*/

        /* その左の子の位置にノードを追加 */
        node->left = mallocNode(number, name);

        /* 追加完了したので処理終了 */
        break;
      }

              /* 左ノードを辿ったことを覚えておく */
      branch[num_branch] = TREE_LEFT;
      num_branch++;
      
      /* 左の子がある場合は左の子を新たな注目ノードに設定 */
      node = node->left;

    } else if(number > node->number){
      /* 追加する値がノードの値よりも大きい場合 */

      if(node->right == NULL){
        /* そのノードの右の子が無い場合（もう辿るべきノードが無い場合）*/

        /* その右の子の位置にノードを追加 */
        node->right = mallocNode(number, name);

        /* 追加完了したので処理終了 */
        break;
      }

      /* 右ノードを辿ったことを覚えておく */
      branch[num_branch] = TREE_RIGHT;
      num_branch++;

      /* 右の子がある場合は右の子を新たな注目ノードに設定 */
      node = node->right;
    } else {
      /* 追加する値とノードの値が同じ場合 */

      printf("%d already exist\n", number);
      break;
    }
  }
    
  return balancing(root, root, NULL, 0, branch, num_branch);
}

/* searchNode:指定されたnumberを持つノードを探索する
   引数１ root : 探索を開始するノードのアドレス
   引数２ number : 探索する会員番号
   返却値 : number を持つノードのアドレス（存在しない場合は NULL）*/
struct node_t *searchNode(struct node_t *root, int number){
  struct node_t *node;
  
  node = root;

  /* 探索を行うループ（注目ノードがNULLになったら終了 */
  while(node){
    if(number < node->number){
      /* 探索値がノードの値よりも小さい場合 */

      /* 注目ノードを左の子ノードに設定 */
      node = node->left;
    } else if(number > node->number){
      /* 探索値がノードの値よりも大きい場合 */

      /* 注目ノードを右の子ノードに設定 */
      node = node->right;
    } else {
      /* 探索値 = ノードの値の場合 */
      return node;
    }
  }
  
  /* 探索値を持つノードが見つからなかった場合 */
  return NULL;
}

/* deleteNoChildeNode:指定された子の無いノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   引数３ parent：削除するノードの親ノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteNoChildNode(struct node_t *root, struct node_t *node, struct node_t *parent){

  if(parent != NULL){
    /* 親がいる場合（根ノード以外の場合）は
    削除対象ノードを指すポインタをNULLに設定 */
    if(parent->left ==  node){
       /* 削除対象ノードが親ノードから見て左の子の場合 */
        parent->left = NULL;
    } else {
      /* 削除対象ノードが親ノードから見て右の子の場合 */
      parent->right = NULL;
    }
    free(node);
  }  else {
    /* 削除対象ノードが根ノードの場合 */
    free(node);
      
    /* 根ノードを指すポインタをNULLに設定 */
    root = NULL;
  }
    
  return root;
}

/* deleteOneChildeNode:指定された子が一つのノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   引数３ child : 削除するノードの子ノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteOneChildNode(struct node_t *root, struct node_t *node, struct node_t * child){
   
  /* 削除対象ノードにその子ノードのデータとポインタをコピー */
  node->number = child->number;
  strcpy(node->name, child->name);
  node->left = child->left;
  node->right = child->right;
    
  /* コピー元のノードを削除 */
  free(child);
  
  return root;
}

/* deleteTwoChildeNode:指定された子が二つのノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ node : 削除するノードのアドレス
   返却値 : 根ノードのアドレス */
struct node_t *deleteTwoChildNode(struct node_t *root, struct node_t *node, int *branch, int *num_branch){

  struct node_t *max;
  struct node_t *maxParent;
  
  /* 左の子から一番大きい値を持つノードを探索 */
  max = node->left;
  maxParent = node;

  /* 左の子ノードを辿ったことを覚えておく */
  branch[*num_branch] = TREE_LEFT;
  (*num_branch)++;
    
  while(max->right != NULL){
    maxParent = max;
    max = max->right;

    /* 右の子ノードを辿ったことを覚えておく */
    branch[*num_branch] = TREE_RIGHT;
    (*num_branch)++;
  }
  printf("max number is %d\n", max->number);
    
  /* 最大ノードのデータのみ削除対象ノードにコピー */
  node->number = max->number;
  strcpy(node->name, max->name);
    
  /* 最大ノードを削除 */
  
  /* maxは最大ノードなので必ずmax->rightはNULLになる */
  if(max->left == NULL){
    /* 最大ノードに子がいない場合 */
    root = deleteNoChildNode(root, max, maxParent);      
    } else {
      /* 最大ノードに子供が一ついる場合 */
      root = deleteOneChildNode(root, max, max->left);
    }

    return root;
}



/* deleteNode:指定されたnumberを持つノードを削除する
   引数１ root : 木の根ノードのアドレス
   引数２ number : 削除する会員番号
   返却値 : 根ノードのアドレス */
struct node_t *deleteNode(struct node_t *root, int number){
  struct node_t *node;
  struct node_t *parent;
  int branch[MAX_HEIGHT] = {0};
  int num_branch = 0;

  if(root == NULL){
    return NULL;
  }

  /* 削除対象ノードを指すノードを探索 */
  node = root;
  parent = NULL;
  
  while(node !=  NULL){
    if(number < node->number){
      parent = node;
      node = node->left;

      /* 左の子ノードを辿ったことを覚えておく */
      branch[num_branch] = TREE_LEFT;
      num_branch++;
    } else if(number > node->number){
      parent = node;
      node = node->right;

      /* 右の子ノードを辿ったことを覚えておく */
      branch[num_branch] = TREE_RIGHT;
      num_branch++;
    } else {
      break;
    }
  }
  
  /* 指定されたnumberを値として持つノードが存在しない場合は何もせず終了 */
  if(node == NULL){
    printf("%d を持つノードが存在しません\n", number);
    return root;
  }

  printf("Delete %d(%s) node\n", node->number, node->name);

  if(node->left == NULL && node->right == NULL){    
    /* 子がいないノードの削除 */
    root = deleteNoChildNode(root, node, parent);
  } else if((node->left != NULL && node->right == NULL) ||
    (node->right != NULL && node->left == NULL)){
    /* 子が一つしかない場合 */
    
    if(node->left != NULL){
      root = deleteOneChildNode(root, node, node->left);
    } else {
      root = deleteOneChildNode(root, node, node->right);
    }
  } else {
    /* 左の子と右の子両方がいるノードの削除 */
    root = deleteTwoChildNode(root, node, branch, &num_branch);
  }
  
  return balancing(root, root, NULL, 0, branch, num_branch);
}

/* printTree:rootを根ノードとする二分探索木をの全ノードを表示する
   引数１ root : 木の根ノードのアドレス
   引数２ depth: 関数呼び出しの深さ
   返却値 : なし */
void printTree(struct node_t *root, int depth){
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
  printf("+%3d(%s)\n", root->number, root->name);

  /* 左の子孫ノードを表示 */
  printTree(root->left, depth+1);

  depth++;
}

int main(void){
  struct node_t *root, *node;
  int input;
  int number;
  char name[MAX_NAME_LEN];
  int loop;

  /* まだ木がないのでrootをNULLにセット */
  root = NULL;

  /* 最初にてきとうにノードを追加しておく */
  root = addNode(root, 100, "100");
  root = addNode(root, 200, "200");
  root = addNode(root, 300, "300");
  root = addNode(root, 50, "50");
  root = addNode(root, 150, "150");
  root = addNode(root, 250, "250");
  root = addNode(root, 10, "1");
  root = addNode(root, 125, "125");
  root = addNode(root, 5, "5");
  root = addNode(root, 25, "25");
  root = addNode(root, 500, "500");
  root = addNode(root, 175, "175");
  root = addNode(root, 501, "501");
  root = addNode(root, 502, "502");
  root = addNode(root, 503, "503");
  root = addNode(root, 504, "504");
  root = addNode(root, 505, "505");
  root = addNode(root, 506, "506");
  root = addNode(root, 507, "507");
  root = addNode(root, 508, "508");
  root = addNode(root, 509, "509");
  root = addNode(root, 510, "510");

  loop = 1;
  while(loop){
    printf("処理を選択(1:add, 2:delete, 3:search, 4:exit)");
    scanf("%d", &input);

    switch(input){
    case 1:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }

      printf("名前:");
      scanf("%s", name);

      root = addNode(root, number, name);
      break;
    case 2:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }

      root = deleteNode(root, number);
      
      break;
    case 3:
      printf("会員番号(1 - 999):");
      scanf("%d", &number);
      if(number < 1 || number > 999){
        printf("値が範囲外です\n");
        continue;
      }
      
      node = searchNode(root, number);
      if(node == NULL){
        printf("number %d is not found\n", number);
      } else {
        printf("number %d : %s\n", number, node->name);
      }
      break;
    default:
      loop = 0;
      break;
    }
    printTree(root, 0);
  }

  deleteTree(root);   
  
  return 0;
}
```



#### 木の巡回

```c : http://www.nct9.ne.jp/m_hiroi/linux/clang13.html
static void foreach_node(void (*func)(double), Node *node)
{
  if (node != NULL) {
    foreach_node(func, node->left);
    func(node->item);
    foreach_node(func, node->right);
  }
}
```

