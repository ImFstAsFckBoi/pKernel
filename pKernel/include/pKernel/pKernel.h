#ifndef PK_H_
#define PK_H_

#include "types.h"
#include "sema.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STACK_SIZE 2048
#define KERNEL_STACK_SIZE 128



typedef union register32
{
    DWORD b32;
    WORD b16;
    BYTE HL[2];
} register32;

typedef enum proc_stat
{
    NEW,
    RUNNING,
    DONE,
    WAITING,
} proc_stat;

typedef struct cpu_state_t
{
    register32 AX; register32 CX; register32 DX;

    register32 BX; register32 SP; register32 BP;
    register32 SI; register32 DI; register32 IP;

    register32 SS; register32 CS; register32 DS;
    register32 ES; register32 FS; register32 GS;

    register32 EFLAGS;
} cpu_state_t;

typedef struct pk_sema pk_sema;

typedef struct pcb_t
{
    char *name;
    cpu_state_t cpu_state;

    BYTE *stack;
    proc_stat status;
    int exit_code;
    pk_sema done_signal;
    pk_list subprocesses;
} pcb_t;

typedef struct pKernel
{
    pk_list plist;
    pcb_t *current;
    pcb_t main_proc;
    pcb_t *(*scheduler)();
} pKernel;

/**
 * @brief Change the CPU context proc.
 * 
 * @param proc The process to set context to.
 */
void pk_ctx_switch(pcb_t *proc);
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
 * @brief Push a QWORD (8 bytes) ths stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return QWORD* New address of SP.
 */
QWORD *pk_stack_pushq(pcb_t *proc, QWORD value);
/**
 * @brief Push a DWORD (4 byte) ths stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return DWORD* New address of SP.
 */
DWORD *pk_stack_pushl(pcb_t *proc, DWORD value);
/**
 * @brief Push a WORD (2 byte) ths stack of proc.
 * 
 * @param proc Process to use as stack.
 * @param value Value to push.
 * @return WORD* New address of SP.
 */
WORD *pk_stack_pushw (pcb_t *proc, WORD  value);
/**
 * @brief Push a BYTE ths stack of proc.
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
 * @brief Relinquish the CPU schedule the next proccess.
 * 
 */
void pk_yield(void);
/**
 * @brief Register a new process to be run.
 * 
 * @param entry_func Function to use as main for the process.
 * @param name Optional name of the function.
 * @param args Arguments (argc and argv) to be passed to the main function.
 * @return pcb_t* Pointer to newly created process.
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
NO_RET void pk_process_loader(proc_func_t func, int argc, char **argv);
/**
 * @brief Default scheduler. A scheduler should:
 *  (1) Only return kernel process once all processes are DONE.
 *  (2) Cycle through all processes in some order.
 *  (3) Not return a process with status DONE or WAITING.
 * 
 * @return pcb_t* The next process to obe scheduled
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
/**
 * @brief Lets a proccess add a subprocess to be executed.
 * 
 * @param entry_func Entry (or main) function of the subprocess.
 * @param args Arguments to be passed.
 * @return pid_t An ID which can be used to identify the subprocess when calling pk_wait().
 */
pid_t pk_exec(proc_func_t entry_func, arg_t args);
/**
 * @brief Wait for a subprocess to finish executing and get its exit code.
 * 
 * @param pid ID of the subprocess to wait for.
 * @return int The exit code of the subprocess.
 */
int pk_wait(pid_t pid);



void pk_fire(void);

#ifdef __cplusplus
}
#endif

#endif  // PK_H_
