#ifndef LOCK_H_
#define LOCK_H_

#include "sema.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief A basic lock implementation.
 * 
 */
typedef struct lock
{
    sema sema;
    pcb_t *holder;
} lock;


/**
 * @brief Initialize a lock.
 * 
 * @param l Pointer to lock to initialize.
 */
void lock_init(lock *l);
/**
 * @brief Free memory held by lock.
 * 
 * @param l Pointer to lock to destroy.
 */
void lock_destroy(lock *l);
/**
 * @brief Lock the lock to the current process.
 * 
 * @param l Pointer to lock to lock.
 */
void lock_acquire(lock *l);
/**
 * @brief Unlock the lock held by current process.
 * 
 * @param l Pointer to lock to unlock.
 */
void lock_release(lock *l);

#ifdef __cplusplus
}
#endif

#endif // LOCK_H_
