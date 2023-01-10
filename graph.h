#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct
{
    int count;
    int **edges;
} graph_t;

graph_t *graph_init();
graph_t *graph_load(const char *filename);
graph_t *graph_set(int e, int edges[][3]);
int get_edge(graph_t *graph, int index, int b);
void graph_free(graph_t *graph);

#endif