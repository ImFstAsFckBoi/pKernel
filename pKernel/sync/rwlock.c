#include "rwlock.h"


void rwlock_init(rwlock *rwl)
{
    rwl->b = 0;
    sema_init(&rwl->r, 1);
    sema_init(&rwl->g, 1);
}

void rwlock_destroy(rwlock *rwl)
{
    sema_destroy(&rwl->r);
    sema_destroy(&rwl->g);
}

void rwlock_begin_read(rwlock *rwl)
{
    sema_down(&rwl->r);
    ++rwl->b;
    if (rwl->b == 1)
        sema_down(&rwl->g);

    sema_up(&rwl->r);
}

void rwlock_end_read(rwlock *rwl)
{
    sema_down(&rwl->r);
    --rwl->b;
    if (rwl->b == 0)
        sema_up(&rwl->g);

    sema_up(&rwl->r);
}

void rwlock_begin_write(rwlock *rwl)
{
    sema_down(&rwl->g);
}

void rwlock_end_write(rwlock *rwl)
{
    sema_up(&rwl->g);
}
