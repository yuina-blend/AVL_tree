#ifndef BINARY_TREE
#define BINARY_TREE
//二分木の構造定義
struct binary_tree_node {
    int value;
    struct binary_tree *right;
    struct binary_tree *left;
};
#endif