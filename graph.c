#include "graph.h"
#include "utils.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

graph_t *graph_init()
{
    graph_t *graph = calloc(1, sizeof(graph_t));
    return graph;
}

graph_t *graph_load(const char *filename)
{
    graph_t *graph = graph_init();
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    char c;
    while ((c = fgetc(file)) < '0' || c > '9')
        fseek(file, -2, SEEK_CUR);
    fseek(file, -1, SEEK_CUR);
    while (fgetc(file) != '\n')
        fseek(file, -2, SEEK_CUR);

    graph->count = fread_int(file) + 1;
    graph->edges = calloc(graph->count, sizeof(int *));

    fseek(file, 0, SEEK_SET);
    
    int index;
    while ((index = fread_int(file)) != EOF)
    {
        int vertex = fread_int(file);
        int cost = fread_int(file);

        int *edges = graph->edges[index];
        if (!graph->edges[index])
        {
            graph->edges[index] = edge_init();
            edges = graph->edges[index];
        }
        else if (edges[0] == edges[1])
        {
            int *temp = realloc(edges, edges[1] * 4 * sizeof(int));
            if (!temp)
            {
                graph_free(graph);
                return false;
            }
            edges = temp;
            graph->edges[index] = temp;
            edges[1] = edges[1] * 2;
        }
        edges[edges[0] * 2 + 2] = vertex;
        edges[edges[0] * 2 + 3] = cost;
        edges[0]++;
    }
    fclose(file);
    return graph;
}

graph_t *graph_set(int e, int edges[][3])
{
    graph_t *graph = graph_init();
    for (size_t i = 0; i < e; i++)
    {
        int index = edges[i][0];
        int vertex = edges[i][1];
        int cost = edges[i][2];

        if (!graph->edges[index])
            graph->edges[index] = edge_init();
        else if (graph->edges[index][0] == graph->edges[index][1])
        {
            int *temp = realloc(graph->edges[index], graph->edges[index][1] * 4 * sizeof(int));
            my_assert(temp != NULL, ERR_MEM);
            graph->edges[index] = temp;
            graph->edges[index][1] = graph->edges[index][1] * 2;
        }
        graph->edges[index][graph->edges[index][0] * 2 + 2] = vertex;
        graph->edges[index][graph->edges[index][0] * 2 + 3] = cost;
        graph->edges[index][0]++;
    }
    return graph;
}

int get_edge(graph_t *graph, int a, int b)
{
    int i = 2;
    if (graph->edges[a])
        for (size_t c = 0; c < graph->edges[a][0]; c++, i += 2)
            if (graph->edges[a][i] == b)
                return graph->edges[a][i + 1];

    return __INT_MAX__;
}

void graph_free(graph_t *graph)
{
    for (size_t i = 0; i < graph->count; i++)
        free(graph->edges[i]);
    free(graph->edges);
    free(graph);
}