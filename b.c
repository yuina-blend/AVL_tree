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

