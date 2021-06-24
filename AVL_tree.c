#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEIGHT 100

#define TREE_LEFT 1
#define TREE_RIGHT 2

// #include "AVL_tree.h"

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
