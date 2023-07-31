
#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "sema.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct cpu_state_t
{
    register32 AX; register32 CX; register32 DX;

    register32 BX; register32 SP; register32 BP;
    register32 SI; register32 DI; register32 IP;

    register32 SS; register32 CS; register32 DS;
    register32 ES; register32 FS; register32 GS;

    register32 EFLAGS;
} cpu_state_t;

typedef enum proc_stat
{
    NEW,
    RUNNING,
    DONE,
    SEMA_WAIT,
    TIME_WAIT
} proc_stat;


typedef struct pcb_t
{
    char *name;
    cpu_state_t cpu_state;

    BYTE *stack;
    proc_stat status;
    int exit_code;
    sema done_signal;
    pk_list subprocesses;
    DWORD wakeup_time;
} pcb_t;

/**
 * @brief Change the CPU context proc.
 * 
 * @param proc The process to set context to.
 */
void pk_ctx_switch(pcb_t *proc);

#ifdef __cplusplus
}
#endif

NO_RET void pk_process_loader(proc_func_t func, int argc, char **argv);
/**
 * @brief Default scheduler. A scheduler should:
 *  (1) Only return kernel process once all processes are DONE.
 *  (2) Cycle through all processes in some order.
 *  (3) Not return a process with status DONE or WAITING.
 * 
 * @return pcb_t* The next process to obe scheduled
 */


#endif // CONTEXT_H_
