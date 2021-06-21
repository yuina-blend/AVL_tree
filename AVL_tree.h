#ifndef BINARY_TREE
#define BINARY_TREE
//AVL木の構造定義
struct ALV_tree_node {
    int value;
    struct AVL_tree_node *right;
    struct AVL_tree_node *left;
};
#endif