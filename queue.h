#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct node
{
    unsigned long long key;
    int val;
} node_t;

typedef struct
{
    int cnt;
    int cap;
    node_t *arr;
    int *lookup;
} queue_t;

queue_t *queue_init();
int queue_insert(queue_t *queue, unsigned long long key, int val);
int queue_pop(queue_t *queue);
void queue_decrese_key(queue_t *queue, int val, unsigned long long new);
void queue_free(queue_t **queue_p);

#endif
