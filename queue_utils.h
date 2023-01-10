#ifndef __QUEUE_UTILS_H__
#define __QUEUE_UTILS_H__

#include "queue.h"

void move_children_to_root(queue_t *queue, node_t *node);
node_t *extract_node(node_t *node);
void node_insert(node_t *node, node_t *add);
node_t *find_val(node_t *node, node_t *start, int val);
node_t *create_node(int key, int val);
node_t *get_parrent(node_t *node);
void queue_print(queue_t *queue);
void node_print(node_t *node, int level);
void add_child(node_t *parrent, node_t *child);
void remove_child(node_t *parrent, node_t *child);
void shift_to_root(queue_t *queue, node_t *node);

#endif