#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct
{
    int cnt;
    int **edges;
} graph_t;

graph_t *allocate_graph();
void load_txt(const char *fname, graph_t *graph);
void load_bin(const char *fname, graph_t *graph);
void save_txt(const graph_t *const graph, const char *fname);
void save_bin(const graph_t *const graph, const char *fname);
graph_t *graph_set(int e, int edges[][3]);
int get_edge(graph_t *graph, int index, int b);
void free_graph(graph_t **graph);
void edges_init(graph_t *graph, int count);

#endif
