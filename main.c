#include <stdio.h>
#include <stdlib.h>

node *malloc_node(int);
node *add_node(node *, int);

int main() {
    node *root = NULL;
    root = add_node(root, 60);
    root = add_node(root, 40);
    root = add_node(root, 30);
    root = add_node(root, 10);
    root = add_node(root, 50);
    root = add_node(root, 20);
}