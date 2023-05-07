#ifndef SEMA_H_
#define SEMA_H_

#include "types.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pcb_t pcb_t;

typedef struct pk_sema
{
    int count;
    pk_list waiting;
} pk_sema;

void sema_init(pk_sema *sema, int count);

void sema_destroy(pk_sema *sema);

void sema_up(pk_sema *sema);

void sema_down(pk_sema *sema);

#ifdef __cplusplus
}
#endif

#endif // SEMA_H_
