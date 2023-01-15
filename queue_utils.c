#include "queue_utils.h"

#include <stdio.h>
#include <math.h>

void swap(queue_t *queue, int a, int b)
{
    node_t *aptr = queue->arr + a;
    node_t *bptr = queue->arr + b;
    unsigned long long temp = aptr->key;
    aptr->key = bptr->key;
    bptr->key = temp;
    temp = aptr->val;
    aptr->val = bptr->val;
    bptr->val = temp;
    temp = queue->lookup[aptr->val];
    queue->lookup[aptr->val] = queue->lookup[bptr->val];
    queue->lookup[bptr->val] = temp;
}

void bubble_up(queue_t *queue, int node)
{
    if (node != 0)
    {
        int parrent = get_parrent(queue, node);
        if (queue->arr[parrent].key > queue->arr[node].key)
        {
            swap(queue, node, parrent);
            bubble_up(queue, parrent);
        }
    }
}

void bubble_down(queue_t *queue, int node)
{
    int left = get_left(queue, node);
    int right = get_right(queue, node);
    int min = node;
    if (left < queue->cnt && queue->arr[left].key < queue->arr[min].key)
        min = left;
    if (right < queue->cnt && queue->arr[right].key < queue->arr[min].key)
        min = right;
    if (min != node)
    {
        swap(queue, node, min);
        bubble_down(queue, min);
    }
}

int get_parrent(queue_t *queue, int node)
{
    return (node - 1) >> 1;
}

int get_left(queue_t *queue, int node)
{
    return (node << 1) + 1;
}

int get_right(queue_t *queue, int node)
{
    return (node << 1) + 2;
}

int queue_find(queue_t *queue, int val)
{
    return queue->lookup[val];
}

void queue_print(queue_t *queue)
{
    int counter = 0;
    int size = 1;
    for (size_t i = 0; i < queue->cnt; i++)
    {
        printf("%zu|%llu ", i, queue->arr[i].key);
        if (++counter == size)
        {
            size <<= 1;
            counter = 0;
            putchar('\n');
        }
    }
    printf("\n\n");
}
