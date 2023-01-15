#ifndef __UTILS_H__
#define __UTILS_H__

#include "graph.h"
#include "dijkstra.h"

#include <stdio.h>

void my_assert(int condition, char *message);
long get_file_size(int fd);
int bread(int fd, char *buffer, size_t size, long off, int b_size);
int sread_num(char **ptr);
char* swrite_num(char *ptr, int num);
int fread_int(FILE *file);
int *edge_init();
void print_dist(dijkstra_t *dij);
int get_last_index(FILE *file);
int get_vertex_count(int fd);

#endif
