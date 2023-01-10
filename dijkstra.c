#include "queue_utils.h"
#include "dijkstra.h"
#include "queue.h"
#include "utils.h"
#include "constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

void throw(int error, void *dijkstra);

void *dijkstra_init(void)
{
    dijkstra_t *dijkstra = calloc(1, sizeof(dijkstra_t));
    my_assert(dijkstra != NULL, ERR_MEM);
    return dijkstra;
}

_Bool dijkstra_load_graph(const char *filename, void *dijkstra)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    dij->graph = graph_load(filename);
    return dij->graph != NULL;
}

_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
{
    ((dijkstra_t *)dijkstra)->graph = graph_set(e, edges);
    return true;
}

_Bool dijkstra_solve(void *dijkstra, int label)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    graph_t *graph = dij->graph;
    dij->dist = malloc(graph->count * sizeof(int));
    dij->prev = malloc(graph->count * sizeof(int));
    my_assert(dij->dist != NULL && dij->prev != NULL, ERR_MEM);
    dij->dist[label] = 0;

    queue_t *queue = queue_init();
    for (size_t i = 0; i < graph->count; i++)
    {
        dij->dist[i] = UINT_MAX;
        if (i == label)
            dij->dist[i] = 0;
        dij->prev[i] = -1;
        queue_insert(queue, dij->dist[i], i);
    }

    while (queue->count != 0)
    {
        int vertex = queue_pop(queue);
        int j = 2;
        for (size_t i = 0; i < graph->edges[vertex][0]; i++, j += 2)
        {
            int neighbor = graph->edges[vertex][j];
            //printf("%d ", neighbor);
            unsigned int detour;
            if (dij->dist[vertex] == UINT_MAX)
                detour = UINT_MAX;
            else
                detour = dij->dist[vertex] + get_edge(graph, vertex, neighbor);
            if (detour < dij->dist[neighbor])
            {
                dij->dist[neighbor] = detour;
                dij->prev[neighbor] = vertex;
                node_t *neighbor_node = queue_find(queue, neighbor);
                queue_decrese_key(queue, neighbor_node, detour);
            }
        }
        // printf("\nvertex: %d\n", vertex);
        // for (size_t i = 0; i < dij->graph->count; i++)
        // {
        //     printf("%zu: %u\n", i, dij->dist[i]);
        // }
        // putchar('\n');
    }
    queue_free(queue);
    return false;
}

_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3])
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    for (size_t i = 0; i < dij->graph->count; i++)
    {
        solution[i][0] = i;
        solution[i][0] = dij->dist[i];
        solution[i][0] = dij->prev[i];
    }
    return true;
}

_Bool dijkstra_save_path(const void *dijkstra, const char *filename)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    FILE *file = fopen(filename, "w");
    if (!file)
        return false;
    for (size_t i = 0; i < dij->graph->count; i++)
        fprintf(file, "%zu %d %d\n", i, dij->dist[i], dij->prev[i]);
    fclose(file);
    return true;
}

void dijkstra_free(void *dijkstra)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    graph_free(dij->graph);
    free(dij->dist);
    free(dij->prev);
    free(dijkstra);
}
