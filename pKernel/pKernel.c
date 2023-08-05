#include "pKernel.h"
#include "debug.h"
#include "context.h"
#include "time.h"
#include "utils.h"
#include "dbg.h"

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
    kernel.main_proc.time_info.start_time = time_msecs();
    kernel.main_proc.name = "kernel";
    kernel.main_proc.stack = SP + KERNEL_STACK_SIZE;
    kernel.main_proc.time_info.deadline = __deadline_default;
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

double calc_t_score(pcb_t *p)
{
    double x = p->time_info.cpu_time * (p->time_info.deadline / __deadline_default) * (1 + (p == kernel.current));

    switch (p->status)
    {
    case DONE:
    case SEMA_WAIT:
        return T_SCORE_MAX;
    case TIME_WAIT:
        x += (int)p->time_info.wakeup_time - (int)time_msecs();
    case NEW:
    case RUNNING:
        break;
    default:
        pk_panic("Unhandled process status in scheduler.");
    }

    return x;
}

pcb_t *pk_default_next_proc(void)
{
    dbg_location;
    const size_t sz = kernel.plist.size;
    if (kernel.current == &kernel.main_proc)
        return (pcb_t *) list_at(&kernel.plist, 0);
    else if (sz == 1)
    {
        switch (kernel.current->status)
        {
        case DONE:
        case SEMA_WAIT:
            return &kernel.main_proc;
        case NEW:
        case RUNNING:
            return kernel.current;
        case TIME_WAIT:
        {
            if (kernel.current->time_info.wakeup_time > time_msecs())
                busy_sleep(kernel.current->time_info.wakeup_time - time_msecs());
            return kernel.current;
        }
        default:
            pk_panic("Unhandled process status in scheduler.");
        }
    }


    struct pair {
        double t_score;
        pcb_t *p;
    };

    struct pair *t_scores = malloc(sizeof(struct pair) * sz);
    struct pair *pick = t_scores;
    for (size_t i = 0; i < sz; ++i)
    {
        struct pair *p = t_scores+i;
        p->p = (pcb_t *)list_at(&kernel.plist, i);
        p->t_score = calc_t_score(p->p);

        if (p->t_score < pick->t_score)
            pick = p;
    }

    asm volatile ("" : : : "memory");
    char s[128];
    sprintf(s, "Moew %e\n", pick->t_score);

    //for (size_t i = 0; i < sz - 1; ++i)
    //for (size_t j = 0; j < sz - i - 1; ++j)
    //if (t_scores[j].t_score > t_scores[j+1].t_score)
    //    swap(t_scores+j, t_scores+j+1);
    pcb_t *out;

    switch (pick->p->status)
    {
    case DONE:
    case SEMA_WAIT:
        out = &kernel.main_proc;
        break;
    case TIME_WAIT:
        if (pick->p->time_info.wakeup_time > time_msecs())
            busy_sleep(pick->p->time_info.wakeup_time - time_msecs());
        out = pick->p;
        break;
    case NEW:
    case RUNNING:
        out = pick->p;
        break;
    default:
        pk_panic("Unhandled process status in scheduler.");
    }

    dbg_location;
    free(t_scores);
    return out;
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
