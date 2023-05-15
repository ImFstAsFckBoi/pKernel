#ifndef SEMA_H_
#define SEMA_H_

#include "types.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pcb_t pcb_t;


/**
 * @brief Simple semaphore implementation.
 * 
 */
typedef struct sema
{
    int count;
    pk_list waiting;
} sema;


/**
 * @brief Initialize a semaphore.
 * 
 * @param sema Pointer to the semaphore to initialize.
 * @param count Counter value to start with.
 */
void sema_init(sema *sema, int count);
/**
 * @brief Free the semaphores memory.
 * 
 * @param sema Pointer to the semaphore to destroy.
 */
void sema_destroy(sema *sema);
/**
 * @brief Increment the semaphores counter.
 * 
 * @param sema Pointer to the semaphore.
 */
void sema_up(sema *sema);
/**
 * @brief Decrement the semaphores counter.
 * If 0, place process into a waiting state.
 * 
 * @param sema Pointer to the semaphore.
 */
void sema_down(sema *sema);

#ifdef __cplusplus
}
#endif

#endif // SEMA_H_
