
#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "sema.h"
#include "time.h"

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

typedef struct com_info_t
{
    int exit_code;
    sema done_signal;
} com_info_t;

typedef struct time_info_t
{
    msecs_t wakeup_time;
    msecs_t deadline;
} time_info_t;

#define __deadline_default __INT32_MAX__

typedef struct pcb_t
{
    char *name;
    cpu_state_t cpu_state;
    BYTE *stack;
    proc_stat status;
    pk_list subprocesses;
    com_info_t com_info;
    time_info_t time_info;
} pcb_t;

/**
 * @brief Change the CPU context proc.
 * 
 * @param proc The process to set context to.
 */
void pk_ctx_switch(pcb_t *proc);
/**
 * @brief Register a new process to be run.
 *
 * @param entry_func Function to use as main for the process.
 * @param name Optional name of the function.
 * @param args Arguments (argc and argv) to be passed to the main function.
 * If you dont want any args you can pass `NULLARG`, same as `(arg_t){0, NULL}`
 * @return pcb_t* Pointer to newly created process.
 */
NO_RET void pk_process_loader(proc_func_t func, int argc, char **argv);

#ifdef __cplusplus
}
#endif



#endif // CONTEXT_H_
