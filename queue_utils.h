#ifndef __QUEUE_UTILS_H__
#define __QUEUE_UTILS_H__

#include "queue.h"

void swap(queue_t *queue, int a, int b);
void bubble_up(queue_t *queue, int node);
void bubble_down(queue_t *queue, int node);
int get_parrent(queue_t *queue, int node);
int get_left(queue_t *queue, int node);
int get_right(queue_t *queue, int node);
void queue_print(queue_t *queue);
int queue_find(queue_t *queue, int val);

#endif
