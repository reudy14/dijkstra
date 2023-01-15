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
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void *dijkstra_init(void)
{
    return calloc(1, sizeof(dijkstra_t));
}

_Bool dijkstra_load_graph(const char *filename, void *dijkstra)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    dij->graph = allocate_graph();
    load_txt(filename, dij->graph);
    if (dij->graph->cnt < 0)
    {
        dij->graph->cnt = -dij->graph->cnt;
        free_graph(&dij->graph);
        return false;
    }
    return true;
}

_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    dij->graph = allocate_graph();
    if (!dij->graph)
        return false;

    edges_init(dij->graph, edges[e - 1][0] + 1);
    for (size_t i = 0; i < e; i++)
    {
        int index = edges[i][0];
        int vertex = edges[i][1];
        int cost = edges[i][2];
        int *edges = dij->graph->edges[index];
        if (edges[0] == edges[1])
        {
            int *temp = realloc(edges, (edges[1] * 4 + 2) * sizeof(int));
            if (!temp)
                return false;
            edges = temp;
            dij->graph->edges[index] = temp;
            edges[1] = edges[1] * 2;
        }
        edges[edges[0] * 2 + 2] = vertex;
        edges[edges[0] * 2 + 3] = cost;
        edges[0]++;
    }

    if (dij->graph->cnt < 0)
    {
        dij->graph->cnt = -dij->graph->cnt;
        return false;
    }
    return true;
}

_Bool dijkstra_solve(void *dijkstra, int label)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    graph_t *graph = dij->graph;
    dij->dist = calloc(graph->cnt, sizeof(unsigned long long));
    if (!dij->dist)
        return false;
    dij->prev = malloc(graph->cnt * sizeof(int));
    if (!dij->prev)
    {
        free(dij->prev);
        return false;
    }

    queue_t *queue = queue_init(graph->cnt);
    for (size_t i = 0; i < graph->cnt; i++)
    {
        dij->dist[i] = ULLONG_MAX;
        if (i == label)
            dij->dist[i] = 0;
        dij->prev[i] = -1;
        queue_insert(queue, dij->dist[i], i);
    }
    
    while (queue->cnt != 0)
    {
        int vertex = queue_pop(queue);
        int j = 2;
        for (size_t i = 0; i < graph->edges[vertex][0]; i++, j += 2)
        {
            int neighbor = graph->edges[vertex][j];
            int cost = graph->edges[vertex][j + 1];
            unsigned long long detour;
            if (dij->dist[vertex] == ULLONG_MAX)
                detour = ULLONG_MAX;
            else
                detour = dij->dist[vertex] + cost;
            if (detour < dij->dist[neighbor])
            {
                dij->dist[neighbor] = detour;
                dij->prev[neighbor] = vertex;
                queue_decrese_key(queue, neighbor, detour);
            }
        }
    }
    queue_free(&queue);
    return true;
}

_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3])
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    if (n != dij->graph->cnt)
        return false;
    for (size_t i = 0; i < dij->graph->cnt; i++)
    {
        solution[i][0] = i;
        solution[i][1] = dij->dist[i];
        solution[i][2] = dij->prev[i];
    }
    return true;
}

_Bool dijkstra_save_path(const void *dijkstra, const char *filename)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    // int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    FILE *file = fopen(filename, "w");
    if (!file)
        return false;
    char buffer[MAX_WBUF];
    char *max_ptr = buffer + MAX_WBUF - WBUF_MARGIN;
    char *ptr = buffer;
    for (size_t i = 0; i < dij->graph->cnt; i++)
    {
        int temp[3] = {i, dij->dist[i], dij->prev[i]};
        for (size_t i = 0; i < 3; i++)
        {
            ptr = swrite_num(ptr, temp[i]);
            *ptr = i == 2 ? '\n' : ' ';
            if (ptr++ > max_ptr)
            {
                fwrite(buffer, sizeof(char), ptr - buffer, file);
                ptr = buffer;
            }
        }
    }
    if (ptr > buffer)
        fwrite(buffer, sizeof(char), ptr - buffer, file);
    fclose(file);
    return true;
}

void dijkstra_free(void *dijkstra)
{
    dijkstra_t *dij = ((dijkstra_t *)dijkstra);
    if (dij)
    {
        free_graph(&dij->graph);
        if (dij->dist)
            free(dij->dist);
        if (dij->prev)
            free(dij->prev);
        free(dijkstra);
    }
}
