#ifndef PK_H_
#define PK_H_

#include "types.h"
#include "sema.h"
#include "list.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

// https://en.na4.teamsupport.com/knowledgeBase/16977436
// Stack needs to be sufficiently large to allow use of 
// functions like printf which uses circa 2k bytes on stack
// to small stack will lead to bugs crazy bugs
// seems to be a sane minimum
#define STACK_SIZE KiB(4)           // Max size of heap allocated stack for processes
#define KERNEL_STACK_SIZE KiB(4)    // Size of "preallocated" kernel stack
#define T_SCORE_MAX __FLT64_MAX__

typedef struct sema sema;

typedef struct pKernel
{
    pk_list plist;
    pcb_t *current;
    pcb_t main_proc;
    pcb_t *(*scheduler)();
} pKernel;


/**
 * @brief Push N bytes to the stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param buff Buffer to push.
 * @param len Number of bytes.
 * @return void* New address of SP. Should be previous SP - len.
 */
void *pk_stack_cpy(pcb_t *proc, void *buff, size_t len);
/**
 * @brief Push a QWORD (8 bytes) to the stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return QWORD* New address of SP.
 */
QWORD *pk_stack_pushq(pcb_t *proc, QWORD value);
/**
 * @brief Push a DWORD (4 byte) to the stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return DWORD* New address of SP.
 */
DWORD *pk_stack_pushl(pcb_t *proc, DWORD value);
/**
 * @brief Push a WORD (2 byte) to the stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return WORD* New address of SP.
 */
WORD *pk_stack_pushw (pcb_t *proc, WORD  value);
/**
 * @brief Push a BYTE to the stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return BYTE* New address of SP.
 */
BYTE *pk_stack_push (pcb_t *proc, BYTE  value);
/**
 * @brief Initialize kernel
 * 
 * @param scheduler function that returns next process (pcb_t *) to run,
 * NULL uses default. Scheduler must:
 *  (1) Only return kernel process once all processes are DONE.
 *  (2) Cycle through all processes in some order.
 *  (3) Not return a process with status DONE or WAITING.
 */
void pk_init(pcb_t *(*scheduler)(void));
/**
 * @brief Default scheduler. A scheduler should:
 *  (1) Only return kernel process once all processes are DONE or SEMA_WAIT.
 *  (2) Cycle through all processes in some order.
 *  (3) Not return a process with status DONE or TIME_WAIT.
 * 
 * @return pcb_t* The next process to obe scheduled.
 */

pcb_t *pk_add_proc(proc_func_t entry_func, char *name, arg_t args);
/**
 * @brief Start the kernel and run until a crash or all processes finish.
 * 
 */
void pk_run(void);
/**
 * @brief First code to be run in a NEW process.
 * Loads the cpu context, changes status to RUNNING,
 * sets return addresses to pk_exit_handler
 * and jumps to the processes main function.
 * 
 * @param func Function to jump to when done.
 * @param argc argc to be passed to func.
 * @param argv argv to be passed to func.
 */
pcb_t *pk_default_next_proc(void);
/**
 * @brief Cleanups kernel and frees process stacks.
 * 
 */
void pk_cleanup(void);
/**
 * @brief The functions that processes goes to when returning.
 * Handles exit statuses and changes status to DONE.
 *
 */
NO_RET void pk_exit_handler(void);

#ifdef __cplusplus
}
#endif

#endif  // PK_H_
