#include "pKernel.h"
#include "debug.h"
#include "context.h"
#include "time.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


pKernel kernel;

void pk_yield(void)
{
    pk_ctx_switch(kernel.scheduler());
}

void pk_sleep(DWORD msec)
{
    kernel.current->wakeup_time = msecs() + msec;
    kernel.current->status = TIME_WAIT;
    pk_yield();
    kernel.current->status = RUNNING;
}

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
    sema_init(&p->done_signal, 0);
    list_init(&p->subprocesses);
    list_push_back(&kernel.plist, (DWORD)p);

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
    pcb_t **maybes = malloc(sizeof(pcb_t *) * sz);
    memset(maybes, (int) NULL, sizeof(pcb_t *) * sz);

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
                if (p->wakeup_time < wakeup)
                {
                    wait_for = p;
                    wakeup = p->wakeup_time;
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
        printf("Waiting for sleeping process\n");
        if (wakeup > msecs())
            busy_sleep(wakeup - msecs());

        output = wait_for;
        goto ret;
    }

    return &kernel.main_proc;
ret:
    free(maybes);
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

    printf("Kernel exited with process");
    for (size_t i = 0; i < list.size-1; ++i)
    {
        printf(" \"%s\"", (char *)list_at(&list, i));
    }
    printf(" and \"%s\" unfinished.\n", (char *)list_at(&list, list.size-1));

    if (list.size > 1)
    {
        printf("This is likely caused to a deadlock.\n");
    }

    for (size_t i = 0; i < kernel.plist.size; ++i)
    {
        register pcb_t *p = ((pcb_t *)list_at(&kernel.plist, i));

        sema_destroy(&p->done_signal);
        list_destroy(&p->subprocesses);
        free(p->stack - STACK_SIZE);
        free(p);
    }

    list_destroy(&kernel.plist);
}

NO_RET void pk_exit_handler(void)
{
    int ret;
    asm("movl %%EAX, %0" : "=r"(ret) : : "eax");
    pcb_t *p = kernel.current;
    printf("%s: %d\n", p->name, ret);
    p->exit_code = ret;
    p->status = DONE;
    sema_up(&p->done_signal);
    pk_ctx_switch(kernel.scheduler());
    __builtin_unreachable();
}

pid_t pk_exec(proc_func_t entry_func, arg_t  args)
{
    char *parent_name = kernel.current->name;
    register size_t pn_len = strlen(parent_name);

    char *sub_name = malloc(pn_len + 14);
    memcpy(sub_name, parent_name, pn_len);
    memcpy(sub_name + pn_len, " - Subprocess", 14);

    pcb_t *p = pk_add_proc(entry_func, sub_name, args);
    register pk_list *sublist = &kernel.current->subprocesses;
    list_push_back(sublist, (DWORD)p);
    free(sub_name);
    return sublist->size - 1;
}

int pk_wait(pid_t pid)
{
    if (pid < 0)
        return -1;

    pcb_t *p = ((pcb_t *)list_at(&kernel.current->subprocesses, pid));
    sema_down(&p->done_signal);
    int tmp = p->exit_code;
    sema_up(&p->done_signal);
    return tmp;
}
