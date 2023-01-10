#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

typedef struct node
{
    unsigned int key;
    int value;

    bool marked;
    unsigned int degree;
    struct node *parrent;
    struct node *child;
    struct node *left;
    struct node *right;
} node_t;

typedef struct queue
{
    unsigned int count;
    node_t *min;
    node_t *root;
} queue_t;

queue_t *queue_init();
void queue_push(queue_t *queue, node_t *node);
node_t *queue_insert(queue_t *queue, unsigned int key, int val);
int queue_pop(queue_t *queue);
node_t *queue_find(queue_t *queue, int val);
void queue_decrese_key(queue_t*queue, node_t*node, unsigned int new_key);
node_t *list_concat(node_t *a, node_t *b);
void queue_free(queue_t *queue);

#endif
