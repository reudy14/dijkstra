#include "queue_utils.h"
#include "utils.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

void move_children_to_root(queue_t *queue, node_t *node)
{
    if (!node->child)
        return;
    node_t *child = node->child;
    while (true)
    {
        node_t *temp = child;
        child = extract_node(child);
        temp->parrent = NULL;
        queue_push(queue, temp);
        if (!child)
            break;
    }
    node->degree = 0;
    node->child = NULL;
}

node_t *extract_node(node_t *node)
{
    if (!node)
        return NULL;
    if (node->right == node)
    {
        node->left = node;
        node->right = node;
        return NULL;
    }
    node_t *list = node->right;
    node->left->right = node->right;
    node->right->left = node->left;
    node->left = node;
    node->right = node;
    return list;
}

void node_insert(node_t *node, node_t *add)
{
    my_assert(node != NULL && add != NULL, ERR_PARAMS);
    if (node->right == node)
    {
        node->right = add;
        add->left = node;
    }
    else
    {
        node->left->right = add;
        add->left = node->left;
    }
    node->left = add;
    add->right = node;
}

node_t *find_val(node_t *node, node_t *start, int val)
{
    if (!node)
        return NULL;
    if (node->value == val)
        return node;

    node_t *found = NULL;
    if (node->child)
        if ((found = find_val(node->child, node->child, val)) && found->value == val)
            return found;

    if (node->right != start)
    {
        if ((found = find_val(node->right, start, val)) && found->value == val)
            return found;
    }

    return NULL;
}

node_t *create_node(int key, int val)
{
    node_t *node = calloc(1, sizeof(node_t));
    my_assert(node != NULL, ERR_MEM);
    node->key = key;
    node->value = val;
    node->left = node;
    node->right = node;
    return node;
}

node_t *get_parrent(node_t *node)
{
    if (node)
        if (node->parrent)
            if (node->parrent->child)
                return node->parrent;
    return NULL;
}

void add_child(node_t *parrent, node_t *child)
{
    my_assert(parrent != NULL, ERR_PARAMS);
    if (!child)
        return;
    if (parrent->degree == 0)
    {
        parrent->child = child;
        child->parrent = parrent;
    }
    else
        node_insert(parrent->child, child);
    parrent->degree++;
    child->parrent = parrent;
}

void remove_child(node_t *parrent, node_t *child)
{
    parrent->degree--;
    parrent->child = extract_node(child);
}

void shift_to_root(queue_t *queue, node_t *node)
{
    if (get_parrent(node) == NULL)
        return;
    remove_child(get_parrent(node), node);
    node_insert(queue->root, node);
    node->parrent = NULL;
    if (node->key < queue->min->key)
    {
        queue->min = node;
    }

    if (get_parrent(node))
    {
        if (!get_parrent(node)->marked)
            get_parrent(node)->marked = true;
        else
            shift_to_root(queue, get_parrent(node));
    }
}

void queue_print(queue_t *queue)
{
    if (!queue->root)
    {
        printf("EMPTY\n");
        return;
    }
    bool start = true;
    node_t *cursor = queue->root;
    while (start || cursor != queue->root)
    {
        start = false;
        printf("→%u:%d %p\n", cursor->key, cursor->value, (void *)cursor->right);
        node_print(cursor, 0);
        cursor = cursor->right;
    }
    if (queue->min)
        printf("min: %u| val: %d\n", queue->min->key, queue->min->value);
    else
        printf("NO MIN\n");
}

void node_print(node_t *node, int level)
{
    node_t *temp = node->child;
    for (size_t i = 0; i < node->degree; i++)
    {
        for (size_t i = 0; i < level + 1; i++)
            printf("   ");
        printf("→");
        printf("%u:%d\n", temp->key, temp->value);
        if (temp->degree != 0)
        {
            node_print(temp, level + 1);
        }
        temp = temp->right;
    }
}
