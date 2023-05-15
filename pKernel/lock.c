#include "lock.h"
#include "utils.h"
#include "pKernel.h"

extern pKernel kernel;

void lock_init(lock *l)
{
    sema_init(&l->sema, 1);
    l->holder = NULL;
}

void lock_destroy(lock *l)
{
    sema_destroy(&l->sema);
    l->holder = NULL;
}

void lock_acquire(lock *l)
{
    pk_assert(l->holder != kernel.current, "Trying to acquire lock already held by current thread.");
    sema_down(&l->sema);
    l->holder = kernel.current;
}

void lock_release(lock *l)
{
    pk_assert(l->holder == kernel.current, "Trying to release lock already held by another thread.");
    l->holder = NULL;
    sema_up(&l->sema);
}
