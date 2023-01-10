#include "queue.h"
#include "queue_utils.h"
#include "utils.h"
#include "constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

queue_t *queue_init()
{
    queue_t *queue = calloc(1, sizeof(queue_t));
    queue->min = NULL;
    my_assert(queue != NULL, ERR_MEM);
    return queue;
}

void queue_push(queue_t *queue, node_t *node)
{
    my_assert(queue != NULL && node != NULL, ERR_PARAMS);
    if (queue->root)
        node_insert(queue->root, node);
    else
        queue->root = node;
    if (!queue->min || node->key < queue->min->key)
        queue->min = node;
}

node_t *queue_insert(queue_t *queue, unsigned int key, int val)
{
    node_t *node = create_node(key, val);
    queue_push(queue, node);
    queue->count++;
    return node;
}

int queue_pop(queue_t *queue)
{
    if (queue->count == 0)
        return -1;
    node_t *min = queue->min;
    int val = min->value;
    move_children_to_root(queue, min);
    queue->root = extract_node(min);
    free(min);
    queue->min = NULL;
    queue->count--;

    if (queue->count == 0)
        return val;

    int max_degree = (int)log2(queue->count) + 2;
    node_t **nodes = calloc(max_degree, sizeof(node_t *));

    node_t *roots = queue->root;
    while (roots != NULL)
    {
        node_t *temp = roots;
        roots = extract_node(roots);

        while (nodes[temp->degree] != NULL)
        {
            int degree = temp->degree;

            node_t *peer = nodes[degree];
            node_t *parrent = temp->key < peer->key ? temp : peer;
            node_t *child = temp->key < peer->key ? peer : temp;
            add_child(parrent, child);

            nodes[degree] = NULL;
            temp = parrent;
        }
        nodes[temp->degree] = temp;
    }
    queue->root = NULL;

    for (size_t i = 0; i < max_degree; i++)
        if (nodes[i])
            queue_push(queue, nodes[i]);
    free(nodes);
    return val;
}

node_t *queue_find(queue_t *queue, int val)
{
    return find_val(queue->root, queue->root, val);
}

void queue_decrese_key(queue_t *queue, node_t *node, unsigned int new_key)
{
    if (!node)
        return;
    if (new_key >= node->key)
        return;
    node->key = new_key;
    node_t *parrent = get_parrent(node);
    if (!parrent || parrent->key <= new_key)
        return;

    shift_to_root(queue, node);
}

node_t *list_concat(node_t *a, node_t *b)
{
    node_t *a_last = a->left;
    a_last->right = b;
    a->left = b->left;
    b->left->right = a;
    b->left = a_last;
    return a;
}

void queue_free(queue_t *queue)
{
    while (queue->count != 0)
        queue_pop(queue);
    free(queue);
}
