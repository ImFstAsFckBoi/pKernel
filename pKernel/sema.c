#include "utils.h"
#include "sema.h"
#include "pKernel.h"

extern pKernel kernel;

void sema_init(sema *sema, int count)
{
    sema->count = count;
    list_init(&sema->waiting);
}

void sema_destroy(sema *sema)
{
    pk_assert(!sema->waiting.size, "Attempting to destroy a semaphore still being waited for.");
    list_destroy(&sema->waiting);
}

void sema_up(sema *sema)
{
    ++sema->count;

    if (sema->waiting.size > 0)
    {
        register pcb_t *p = (pcb_t *) list_pop_front(&sema->waiting);
        p->status = RUNNING;
        pk_ctx_switch(p);
    }
}

void sema_up_no_yield(sema *sema)
{
    ++sema->count;

    if (sema->waiting.size > 0)
    {
        register pcb_t *p = (pcb_t *) list_pop_front(&sema->waiting);
        p->status = RUNNING;
    }
}

void sema_down(sema *sema)
{
    if (sema->count == 0)
    {
        list_push_back(&sema->waiting, (DWORD)kernel.current);
        kernel.current->status = WAITING;
        pk_yield();
    }

    --sema->count;
}
