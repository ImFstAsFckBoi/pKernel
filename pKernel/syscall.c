#include "syscall.h"
#include "time.h"
#include "string.h"
#include "stdlib.h"
#include "pKernel.h"

#include "stdio.h"

extern pKernel kernel;

void pk_yield(void)
{
    pk_ctx_switch(kernel.scheduler());
}

void pk_sleep(DWORD msec)
{
    kernel.current->time_info.wakeup_time = time_msecs() + msec;
    kernel.current->status = TIME_WAIT;
    pk_yield();
    kernel.current->status = RUNNING;
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
    sema_down(&p->com_info.done_signal);
    int tmp = p->com_info.exit_code;
    sema_up(&p->com_info.done_signal);
    return tmp;
}

int pk_deadline_set(msecs_t msecs)
{
    msecs_t t = time_msecs();
    register msecs_t *d = &kernel.current->time_info.deadline;
    *d = t + msecs;

    if (*d < t)
    {
        *d = __deadline_default;
        return 1;
    }

    return 0;
}

int pk_deadline_meet()
{
    msecs_t t = time_msecs();
    int res = 0;
    if (kernel.current->time_info.deadline < t)
    {
        // TODO: Debug printf("WARNING! Process \"%s\" failed a deadline!\n", kernel.current->name);
        res =  1;
    }

    kernel.current->time_info.deadline = __deadline_default;
    return res;
}
