#ifndef BINARY_TREE
#define BINARY_TREE
//AVL木の構造定義
struct ALV_tree_node {
    int value;
    struct binary_tree *right;
    struct binary_tree *left;
};
#endif