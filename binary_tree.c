#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

//新しいノードを作成
node *malloc_node(int value) {
    node *new_node = (node*)malloc(sizeof(node));

    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

//新しいノードを追加
node *add_node(node *root, int value) {
    node *tmp_node;

    if (root == NULL) {
        root = malloc_node(value);
        return root;
    }

    tmp_node = root;
    while (true) {
        if (value < tmp_node->value) {
            if (tmp_node->left = NULL) {
                tmp_node->left = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->left;
        }
        else if (value > tmp_node->value) {
            if (tmp_node->right == NULL) {
                tmp_right = malloc_node(value);
                break;
            }
            tmp_node = tmp_node->right;
        }
        else {
            printf("既に存在する値です\n");
            break;
        }
        return root;
    }
}