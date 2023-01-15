#include "queue.h"
#include "queue_utils.h"

#include <stdlib.h>

queue_t *queue_init(int size)
{
    queue_t *queue = malloc(sizeof(queue_t));
    if (!queue)
        return NULL;
    if (!(queue->arr = calloc(size, sizeof(node_t))))
        return NULL;
    if (!(queue->lookup = calloc(size, sizeof(int))))
        return NULL;
    queue->cap = size;
    queue->cnt = 0;
    return queue;
}

int queue_insert(queue_t *queue, unsigned long long key, int val)
{
    queue->arr[queue->cnt].key = key;
    queue->arr[queue->cnt].val = val;
    queue->lookup[val] = queue->cnt;
    bubble_up(queue, queue->cnt);
    return queue->cnt++;
}

int queue_pop(queue_t *queue)
{
    if (queue->cnt == 0)
        return -1;
    int val = queue->arr[0].val;

    swap(queue, 0, --queue->cnt);
    bubble_down(queue, 0);
    queue->lookup[val] = -1;
    return val;
}

void queue_decrese_key(queue_t *queue, int val, unsigned long long new)
{
    // for (size_t i = 0; i < queue->cap; i++)
    //     printf("%d: %d\n", i, queue->lookup[i]);
    // printf("%d %d\n\n", val, new);
    int node = queue_find(queue, val);
    queue->arr[node].key = new;
    if (new < queue->arr[get_parrent(queue, node)].key)
    {
        bubble_up(queue, node);
    }
}

void queue_free(queue_t **queue_p)
{
    free((*queue_p)->arr);
    free((*queue_p)->lookup);
    free(*queue_p);
    *queue_p = NULL;
}
