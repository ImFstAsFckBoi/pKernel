#ifndef RWLOCK_H_
#define RWLOCK_H_

/*
    Raynal implementation of readers-writers lock using semaphores
    https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock#Using_two_mutexes
*/

#include "sema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rwlock
{
    int b;
    sema r, g;
} rwlock;

void rwlock_init(rwlock *rwl);
void rwlock_destroy(rwlock *rwl);

void rwlock_begin_read(rwlock *rwl);
void rwlock_end_read(rwlock *rwl);

void rwlock_begin_write(rwlock *rwl);
    void rwlock_end_write(rwlock *rwl);
#ifdef __cplusplus
}
#endif

#endif // RWLOCK_H_
