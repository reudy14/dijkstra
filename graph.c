#include "graph.h"
#include "utils.h"
#include "constants.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/in.h>

graph_t *allocate_graph()
{
    graph_t *graph = calloc(1, sizeof(graph_t));
    return graph;
}

void edges_init(graph_t *graph, int count)
{
    graph->cnt = count;
    graph->edges = calloc(graph->cnt, sizeof(int *));
    if (!graph->edges)
    {
        graph->cnt = -graph->cnt;
        return;
    }
    for (size_t i = 0; i < graph->cnt; i++)
    {
        graph->edges[i] = edge_init();
        if (!graph->edges[i])
        {
            free(graph->edges);
            graph->cnt = -graph->cnt;
            return;
        }
    }
}

long get_buffer_size(long num)
{
    long off = 1;
    while ((num >>= 1) != 0 && (1 << off) < MAX_RBUF)
        off++;
    return 1 << off;
}

void load_txt(const char *fname, graph_t *graph)
{
    int file = open(fname, O_RDONLY);
    long offset = 0;
    if (file == -1)
    {
        graph->cnt = -graph->cnt;
        return;
    }
    edges_init(graph, get_vertex_count(file));

    long b_size = get_buffer_size(get_file_size(file));
    char *buffer = malloc((b_size + 1) * sizeof(char));
    if (!buffer)
    {
        close(file);
        graph->cnt = -graph->cnt;
        return;
    }

    buffer[0] = '0';
    char *c = buffer;
    int old = 0;
    while (*c != EOF)
    {
        c = buffer;
        offset = bread(file, buffer, b_size, offset, b_size);
        int temp[3];
        while (true)
        {
            for (size_t i = 0; i < 3; i++)
                temp[i] = sread_num(&c);

            if (temp[0] <= EOF)
                break;

            if (old != temp[0])
            {
                int *array = realloc(graph->edges[old], ((graph->edges[old][0] << 1) + 2) * sizeof(int));
                if (!array)
                {
                    graph->cnt = -graph->cnt;
                    close(file);
                    return;
                }
                graph->edges[old] = array;
                graph->edges[old][1] = graph->edges[old][0];
                old = temp[0];
            }

            int *edges = graph->edges[temp[0]];
            if (edges[0] == edges[1])
            {
                int *array = realloc(edges, ((edges[1] << 2) + 2) * sizeof(int));
                if (!array)
                {
                    graph->cnt = -graph->cnt;
                    close(file);
                    return;
                }
                edges = array;
                graph->edges[temp[0]] = array;
                edges[1] *= 2;
            }

            edges[edges[0] * 2 + 2] = temp[1];
            edges[edges[0] * 2 + 3] = temp[2];
            edges[0]++;
        }
    }
    free(buffer);
    close(file);
}

void load_bin(const char *fname, graph_t *graph)
{
    FILE *file = fopen(fname, "rb");
    if (!file)
    {
        graph->cnt = -graph->cnt;
        return;
    }

    fseek(file, -12, SEEK_END);
    fread(&graph->cnt, sizeof(int), 1, file);
    fseek(file, 0, SEEK_SET);
    edges_init(graph, ntohl(graph->cnt) + 1);
    if (graph->cnt < 0)
    {
        fclose(file);
        return;
    }

    unsigned int buffer[3];
    while (fread(buffer, sizeof(int), 3, file) == 3)
    {
        for (size_t i = 0; i < 3; i++)
            buffer[i] = ntohl(buffer[i]);

        int *edges = graph->edges[buffer[0]];
        if (edges[0] == edges[1])
        {
            int *temp = realloc(edges, (edges[1] * 4 + 2) * sizeof(int));
            if (!temp)
            {
                graph->cnt = -graph->cnt;
                fclose(file);
                return;
            }
            edges = temp;
            graph->edges[buffer[0]] = temp;
            edges[1] = edges[1] * 2;
        }

        edges[edges[0] * 2 + 2] = buffer[1];
        edges[edges[0] * 2 + 3] = buffer[2];
        edges[0]++;
    }
    fclose(file);
}

void save_txt(const graph_t *const graph, const char *fname)
{
    FILE *file = fopen(fname, "w");
    my_assert(file != NULL, ERR_FILE);
    for (int i = 0; i < graph->cnt; i++)
    {
        int count = graph->edges[i][0];
        size_t k = 2;
        for (size_t j = 0; j < count; j++, k += 2)
            fprintf(file, "%d %d %d\n", i, graph->edges[i][k], graph->edges[i][k + 1]);
    }
    fclose(file);
}

void save_bin(const graph_t *const graph, const char *fname)
{
    FILE *file = fopen(fname, "wb");
    my_assert(file != NULL, ERR_FILE);
    for (int i = 0; i < graph->cnt; i++)
    {
        int count = graph->edges[i][0];
        size_t k = 2;
        for (size_t j = 0; j < count; j++, k += 2)
        {
            unsigned int buffer[3];
            buffer[0] = htonl(i);
            buffer[1] = htonl(graph->edges[i][k]);
            buffer[2] = htonl(graph->edges[i][k + 1]);

            if (fwrite(buffer, sizeof(int), 3, file) != 3)
            {
                fclose(file);
                my_assert(false, ERR_WRITE);
            }
        }
    }
    fclose(file);
}

int get_edge(graph_t *graph, int a, int b)
{
    int i = 2;
    if (graph->edges[a])
        for (size_t c = 0; c < graph->edges[a][0]; c++, i += 2)
            if (graph->edges[a][i] == b)
                return graph->edges[a][i + 1];

    return UINT_MAX;
}

void free_graph(graph_t **graph_p)
{
    if (!graph_p)
        return;
    if (*graph_p)
    {
        graph_t *graph = *graph_p;
        for (size_t i = 0; i < graph->cnt; i++)
            if (graph->edges[i])
                free(graph->edges[i]);
        if (graph->edges)
            free(graph->edges);
        free(graph);
    }
    *graph_p = NULL;
}
