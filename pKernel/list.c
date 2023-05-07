#include "list.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RESIZE_SCALE 2

void list_init(pk_list *l)
{
    l->data = malloc(8 * sizeof(DWORD));
    l->size = 0;
    l->max = 8;
}

void list_destroy(pk_list *l)
{
    free(l->data);
}

void list_push_back(pk_list *l, DWORD value)
{
    if (l->size >= l->max)
        l->data = realloc(l->data, l->max * RESIZE_SCALE * sizeof(DWORD));

    l->data[l->size++] = value;
}

void list_push_front(pk_list *l, DWORD value)
{
    if (l->size >= l->max)
        l->data = realloc(l->data, l->max * RESIZE_SCALE * sizeof(DWORD));

    memmove(l->data + 1, l->data, l->size * sizeof(DWORD));
    l->data[0] = value;
    ++l->size;
}


DWORD list_pop_back(pk_list *l)
{
    register DWORD tmp = l->data[--l->size];
    l->data[l->size] = 0;
    return tmp;
}

DWORD list_pop_front(pk_list *l)
{
    register DWORD tmp = l->data[0];
    memmove(l->data,
              l->data + 1,
              l->size * sizeof(DWORD));
    l->data[--l->size] = 0;
    return tmp;
}

DWORD list_remove(pk_list *l, size_t pos)
{
    register DWORD tmp = l->data[pos];
    memmove(l->data + pos,
              l->data + pos + 1,
              (l->size - pos - 1) * sizeof(DWORD));
    --l->size;
    return tmp;
}

DWORD list_at(pk_list *l, size_t pos)
{
    return l->data[pos];
}

err_t list_safe_at(pk_list *l, size_t pos, DWORD *value)
{
    if (pos >= l->size)
        return ERR_FAILED;

    *value = l->data[pos];
    return ERR_SUCCESS;
}

size_t list_find(pk_list *l, DWORD value)
{
    for (size_t i = 0; i < l->size; ++i)
    {
        if (l->data[i] == value)
            return i;
    }

    return LIST_NO_POS;
}

void list_print(pk_list *l)
{
    printf("Data: %p\n", (void *)l->data);
    printf("Used: %d Max: %d\n", l->size, l->max);

    for (size_t i = 0; i < l->size; ++i)
        printf("%3d: 0x%02lx\n", i, l->data[i]);
}
