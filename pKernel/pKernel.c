#include "pKernel.h"
#include "debug.h"
#include "context.h"
#include "time.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


pKernel kernel;


pcb_t *pk_add_proc(proc_func_t entry_func, char *name, arg_t args)
{
    register pcb_t *p = malloc(sizeof(pcb_t));
    BYTE *stack = malloc(STACK_SIZE);
    p->cpu_state.SP.b32 = (DWORD)stack + STACK_SIZE;
    p->stack = stack + STACK_SIZE;

    p->name = pk_stack_cpy(p, name, strlen(name) + 1);

    // Load argv data into top of stack
    for (int i = 0; i < args.argc; ++i)
    {
        // Copy each arg to process' own stack
        char *arg = args.argv[i];
        char *ptr = pk_stack_cpy(p, arg, strlen(arg) + 1);
        // Reuse args.argv to remember new address for each arg
        args.argv[i] = ptr;
    }
    char **argv = pk_stack_cpy(p, args.argv, args.argc * sizeof(char *));

    // Load return address and argc and argv

    // Move frame to not include argv data
    // argc and argv list is part of frame
    p->cpu_state.BP.b32 = p->cpu_state.SP.b32;

    pk_stack_pushl(p, (DWORD) argv);
    pk_stack_pushl(p, args.argc);      
    pk_stack_pushl(p, (DWORD) entry_func);

    p->cpu_state.IP.b32 = (DWORD) pk_process_loader;
    p->status = NEW;
    sema_init(&p->com_info.done_signal, 0);
    list_init(&p->subprocesses);
    list_push_back(&kernel.plist, (DWORD)p);
    p->time_info.deadline = __deadline_default;
    p->time_info.wakeup_time = 0;
    return p;
}

void *pk_stack_cpy(pcb_t *proc, void *buff, size_t len)
{
    proc->cpu_state.SP.b32 -= len;
    memcpy((void *)proc->cpu_state.SP.b32, buff, len);
    return (void *)proc->cpu_state.SP.b32;
}

QWORD *pk_stack_pushq(pcb_t *proc, QWORD value)
{
    proc->cpu_state.SP.b32 -= 8;
    ((QWORD *)proc->cpu_state.SP.b32)[0] = value; 
    return (QWORD *)proc->cpu_state.SP.b32;
}

DWORD *pk_stack_pushl(pcb_t *proc, DWORD value)
{
    proc->cpu_state.SP.b32 -= 4;
    ((DWORD *)proc->cpu_state.SP.b32)[0] = value; 
    return (DWORD *)proc->cpu_state.SP.b32;
}

WORD *pk_stack_pushw (pcb_t *proc, WORD  value)
{
    proc->cpu_state.SP.b32 -= 2;
    ((WORD *)proc->cpu_state.SP.b32)[0] = value;
    return (WORD *)proc->cpu_state.SP.b32;
}

BYTE *pk_stack_push (pcb_t *proc, BYTE value)
{
    --proc->cpu_state.SP.b32;
    ((BYTE *)proc->cpu_state.SP.b32)[0] = value; 
    return (BYTE *)proc->cpu_state.SP.b32;
}


void pk_init(pcb_t *(*scheduler)(void))
{
    BYTE *SP;
    asm("movl %%ESP, %0"
        : "=r"(SP));

    list_init(&kernel.plist);
    kernel.main_proc.name = "kernel";
    kernel.main_proc.stack = SP + KERNEL_STACK_SIZE;
    kernel.current = &kernel.main_proc;

    if (scheduler == NULL)
        kernel.scheduler = pk_default_next_proc;
    else
        kernel.scheduler = scheduler;
}

void pk_run(void)
{
    pk_ctx_switch(kernel.scheduler());
    pk_cleanup();
}

pcb_t *pk_default_next_proc(void)
{
    pcb_t *output;
    size_t sz = kernel.plist.size;
    if (kernel.current == &kernel.main_proc)
        return (pcb_t *) list_at(&kernel.plist, 0);
    
    

    size_t offset = list_find(&kernel.plist, (DWORD) kernel.current);
    


    size_t maybe_idx = 0;
    pcb_t **ordered = malloc(sizeof(pcb_t *) * sz);
    list_ncopy(&kernel.plist, ordered, sz);


    for (size_t i = 0; i < sz - 1; ++i)
    for (size_t j = 0; j < sz - i - 1; ++j)   
    if (ordered[j]->time_info.deadline > ordered[j + 1]->time_info.deadline)
        swapl((DWORD *)(ordered+j), (DWORD *)(ordered+j+1));

    pcb_t **maybes = malloc(sizeof(pcb_t *) * sz);
    memset(maybes, (int) NULL, sizeof(pcb_t *) * sz);
    
    if (kernel.plist.size == 1)
    {
        switch (kernel.current->status)
        {
        case DONE:
        case SEMA_WAIT:
            output = &kernel.main_proc;
            goto ret;
        case NEW:
        case RUNNING:
            output = kernel.current;
            goto ret;
        case TIME_WAIT:
            maybes[maybe_idx++] = kernel.current;
        default:
            break;
        }
    }

    for (size_t i = (offset + 1) % sz; i != offset; i = ((i + 1) % sz))
    {
        pcb_t *p = (pcb_t *)list_at(&kernel.plist, i);
        switch (p->status)
        {
        case DONE:
        case SEMA_WAIT:
            break;
        case NEW:
        case RUNNING:
            output = p;
            goto ret;
        case TIME_WAIT:
            maybes[maybe_idx++] = p;
        default:
            break;
        }
    }

    pcb_t *wait_for = NULL;
    size_t wakeup = __INT32_MAX__;
    for (size_t i = 0; i < sz; ++i)
    {
        pcb_t *p = maybes[i];
        if (p != NULL)
        {
            switch (p->status)
            {
            case TIME_WAIT:
                if (p->time_info.wakeup_time < wakeup)
                {
                    wait_for = p;
                    wakeup = p->time_info.wakeup_time;
                }
                break;

            default:
                pk_assert(false, "Unreachable case in switch");
                break;
            }
        }
    }

    if (wait_for != NULL)
    {
        if (wakeup > time_msecs())
            busy_sleep(wakeup - time_msecs());

        output = wait_for;
        goto ret;
    }

    return &kernel.main_proc;
ret:
    free(maybes);
    free(ordered);
    return output;
}

void pk_cleanup(void)
{
    pk_list list;
    list_init(&list);

    for (size_t i = 0; i < kernel.plist.size; ++i)
    {
        register pcb_t *p = ((pcb_t *)list_at(&kernel.plist, i));

        if (p->status != DONE)
        {
            list_push_back(&list, (DWORD)p->name);
        }
    }

    //TODO: Debug
    //printf("Kernel exited with process");
    //for (size_t i = 0; i < list.size-1; ++i)
    //{
    //    printf(" \"%s\"", (char *)list_at(&list, i));
    //}
    //printf(" and \"%s\" unfinished.\n", (char *)list_at(&list, list.size-1));

    //if (list.size > 1)
    //{
    //    printf("This is likely caused to a deadlock.\n");
    //}

    for (size_t i = 0; i < kernel.plist.size; ++i)
    {
        register pcb_t *p = ((pcb_t *)list_at(&kernel.plist, i));

        sema_destroy(&p->com_info.done_signal);
        list_destroy(&p->subprocesses);
        free(p->stack - STACK_SIZE);
        free(p);
    }

    list_destroy(&list);
    list_destroy(&kernel.plist);
}

NO_RET void pk_exit_handler(void)
{
    int ret;
    asm("movl %%EAX, %0" : "=r"(ret) : : "eax");
    pcb_t *p = kernel.current;
    printf("%s: %d\n", p->name, ret);
    p->com_info.exit_code = ret;
    p->status = DONE;
    sema_up(&p->com_info.done_signal);
    pk_ctx_switch(kernel.scheduler());
    __builtin_unreachable();
}

void pk_assert(bool cond, char *msg)
{
    if (cond)
        return;
    
    if (msg != NULL)
        printf("Assertion failed: %s\n Exiting pKernel and continuing main function\n", msg);

    pk_ctx_switch(&kernel.main_proc);
}
