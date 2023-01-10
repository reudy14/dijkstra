#ifndef __UTILS_H__
#define __UTILS_H__

#include "graph.h"
#include "dijkstra.h"

#include <stdio.h>

void my_assert(int condition, char *message);
int fread_int(FILE *file);
int *edge_init();
void print_dist(dijkstra_t *dij);

#endif
