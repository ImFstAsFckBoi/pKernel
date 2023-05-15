#ifndef LOCK_H_
#define LOCK_H_

/*
    Raynal implementation of readers-writers lock using semaphores
    https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock#Using_two_mutexes
*/

#include "sema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lock
{
    sema sema;
    pcb_t *holder;
} lock;

void lock_init(lock *l);
void lock_destroy(lock *l);

void lock_acquire(lock *l);
void lock_release(lock *l);

#ifdef __cplusplus
}
#endif

#endif // LOCK_H_
