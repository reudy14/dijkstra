#include "utils.h"
#include "constants.h"

#include <stdlib.h>
#include <stdbool.h>

void my_assert(int condition, char *message)
{
    if (condition)
        return;
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int fread_int(FILE *file)
{
    char c = fgetc(file);
    while ((c < '0' || c > '9') && c != EOF)
        c = fgetc(file);

    int num = 0;
    while (c != EOF)
    {
        num *= 10;
        num += c - '0';
        if ((c = fgetc(file)) < '0' || c > '9')
            break;
    }
    if (c == EOF)
        return EOF;
    return num;
}

int *edge_init()
{
    int *list = malloc(EDGE_INIT * sizeof(int));
    my_assert(list != NULL, ERR_MEM);
    list[0] = 0;
    list[1] = (EDGE_INIT - 2) / 2;
    return list;
}

void print_dist(dijkstra_t *dij)
{
    for (size_t i = 0; i < dij->graph->count; i++)
        printf("%zu: %d\n", i, dij->dist[i]);
    putchar('\n');
}
