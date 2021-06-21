//二分木の構造定義
typedef struct binary_tree_node {
    int value;
    struct binary_tree *right;
    struct binary_tree *left;
} node;