#ifndef LIST_H_
#define LIST_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIST_NO_POS -1UL;

typedef struct pk_list
{
    DWORD *data;
    size_t size;
    size_t max;
} pk_list;

void list_init(pk_list *l);

void list_destroy(pk_list *l);

void list_push_back(pk_list *l, DWORD value);

void list_push_front(pk_list *l, DWORD value);

DWORD list_pop_back(pk_list *l);

DWORD list_pop_front(pk_list *l);

DWORD list_remove(pk_list *l, size_t pos);

DWORD list_at(pk_list *l, size_t pos);

err_t list_safe_at(pk_list *l, size_t pos, DWORD *value);

size_t list_find(pk_list *l, DWORD value);

void list_print(pk_list *l);

void list_ncopy(pk_list *l, void *dest, size_t n);

#ifdef __cplusplus
}
#endif

#endif // LIST_H_
