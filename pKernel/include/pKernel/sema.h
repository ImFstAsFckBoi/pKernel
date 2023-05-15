#ifndef SEMA_H_
#define SEMA_H_

#include "types.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pcb_t pcb_t;

typedef struct sema
{
    int count;
    pk_list waiting;
} sema;

void sema_init(sema *sema, int count);

void sema_destroy(sema *sema);

void sema_up(sema *sema);

void sema_down(sema *sema);

#ifdef __cplusplus
}
#endif

#endif // SEMA_H_
