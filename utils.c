#include "utils.h"
#include "constants.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void my_assert(int condition, char *message)
{
    if (condition)
        return;
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int bread(int fd, char *buffer, size_t size, long off, int b_size)
{
    int end = pread(fd, buffer, size, off);
    if (end == b_size)
    {
        char c;
        pread(fd, &c, sizeof(char), off + end);
        while (c != '\n')
            pread(fd, &c, sizeof(char), off + --end);
        buffer[end] = EOB;
    }
    else
        buffer[end] = EOF;
    return off + end + 1;
}

long get_file_size(int fd)
{
    struct stat *stats = malloc(sizeof(struct stat));
    fstat(fd, stats);
    long size = stats->st_size;
    free(stats);
    return size;
}

int get_vertex_count(int fd)
{
    long off = get_file_size(fd) - 1;
    char c = '\0';
    while (c != '\n')
        pread(fd, &c, 1, --off);
    off++;
    int num = 0;
    c = '0';
    while (true)
    {
        int res = pread(fd, &c, 1, off++);
        if (res <= 0 || c < '0' || c > '9')
            return num + 1;
        num *= 10;
        num += c - '0';
    }
}

int sread_num(char **ptr)
{
    int num = 0;
    while ((**ptr < '0' || **ptr > '9') && **ptr > EOF)
        (*ptr)++;

    if (**ptr <= EOF)
        return **ptr;
    while (**ptr > EOF)
    {
        num *= 10;
        num += **ptr - '0';
        (*ptr)++;
        if (**ptr < '0' || **ptr > '9')
            break;
    }
    return num;
}

int fread_int(FILE *file)
{
    char c = fgetc(file);
    if (c == EOF)
        return EOF;
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
    return num;
}

int *edge_init()
{
    int *list = malloc(EDGE_INIT * sizeof(int));
    if (!list)
        return NULL;
    list[0] = 0;
    list[1] = (EDGE_INIT - 2) / 2;
    return list;
}

void print_dist(dijkstra_t *dij)
{
    for (size_t i = 0; i < dij->graph->cnt; i++)
        printf("%zu: %llu\n", i, dij->dist[i]);
    putchar('\n');
}

int get_last_index(FILE *file)
{
    fseek(file, 0, SEEK_END);
    char c;
    while ((c = fgetc(file)) < '0' || c > '9')
        fseek(file, -2, SEEK_CUR);
    fseek(file, -1, SEEK_CUR);
    while (fgetc(file) != '\n')
        fseek(file, -2, SEEK_CUR);
    int index = fread_int(file);
    fseek(file, 0, SEEK_SET);
    return index;
}

char *swrite_num(char *ptr, int num)
{
    char reverse[10];
    int r_count = 0;
    if (num == 0)
    {
        *ptr++ = '0';
        return ptr;
    }
    else if (num < 0)
    {
        *ptr++ = '-';
        num = -num;
    }

    while (num != 0)
    {
        reverse[r_count] = num % 10;
        num = (num - reverse[r_count++]) / 10;
    }
    while (r_count--)
        *ptr++ = reverse[r_count] + '0';
    return ptr;
}
