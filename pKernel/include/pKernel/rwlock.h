#ifndef RWLOCK_H_
#define RWLOCK_H_

#include "sema.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Raynal implementation of readers-writer lock using semaphores according to
 * https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock#Using_two_mutexes
 * 
 */
typedef struct rwlock
{
    int b;
    sema r, g;
} rwlock;


/**
 * @brief Initialize the rwlock.
 * 
 * @param rwl Pointer to rwlock to initialize.
 */
void rwlock_init(rwlock *rwl);
/**
 * @brief Free memory held by the rwlock.
 * 
 * @param rwl Pointer to rwlock to destroy.
 */
void rwlock_destroy(rwlock *rwl);
/**
 * @brief Begin a read transaction.
 * 
 * @param rwl Pointer to rwlock.
 */
void rwlock_begin_read(rwlock *rwl);
/**
 * @brief End a read transaction.
 * 
 * @param rwl Pointer to rwlock.
 */
void rwlock_end_read(rwlock *rwl);
/**
 * @brief Begin a write transaction.
 * 
 * @param rwl Pointer to rwlock.
 */
void rwlock_begin_write(rwlock *rwl);
/**
 * @brief End a write transaction.
 * 
 * @param rwl Pointer to rwlock.
 */
void rwlock_end_write(rwlock *rwl);

#ifdef __cplusplus
}
#endif

#endif // RWLOCK_H_
