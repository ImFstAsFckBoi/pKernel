#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

extern pKernel kernel;

DWORD *get_esp()
{
    DWORD *SP;
    asm("movl %%ESP, %0"
        : "=r"(SP));
    return SP;
}

BYTE *get_sp()
{
    BYTE *SP;
    asm("mov %%ESP, %0"
        : "=r"(SP));
    return SP;
}

void print_stack(pcb_t *p, size_t group)
{
    BYTE *SP = (BYTE *)p->cpu_state.SP.b32;

    while (SP <= p->stack)
    {
        printf("%p: 0x", SP);
        SP += group - 1;
        for (size_t _ = 0; _ < group; ++_)
            printf("%02x", *(SP--));
        printf("\n");
        SP += group;
    }
}

DWORD stack_top()
{
    return *get_esp();
}

char *where_is_stack()
{
    /* 
    
    +----------+ 0xFFFFFFFF
    |          |
    |  Main    |
    |  Stack   |
    |          |
    +----------+
    |          |
    |          |
    +----------+ <- sbrk
    |          |
    |  Heap    |
    |          |
    |  (Proc.  |
    |  Stack)  |
    |          |
    +----------+ 0x00000000

    Largest Heap address
    */
    BYTE *heap_top = sbrk(0);

    BYTE *SP = get_sp();

    if (SP > heap_top)
        return "Main";

    for (size_t i = 0; i < kernel.plist.size; ++i)
    {
        register pcb_t *p = (pcb_t *)list_at(&kernel.plist, i);

        if (SP <= p->stack &&
            SP >= p->stack - STACK_SIZE)
            return p->name;
    }

    return "None";
}

void print_memory_layout()
{
    #define sz kernel.plist.size

    typedef struct {
        DWORD addr;
        pcb_t *owner;
    } pair;
    
    pair *addr_list = malloc(sizeof(pair) * (sz + 1) * 2);
    size_t idx = 0;
    addr_list[idx++] = (pair){0xFFFFFFFF, &kernel.main_proc};
    addr_list[idx++] = (pair){kernel.main_proc.cpu_state.SP.b32, &kernel.main_proc};

    for (size_t i = 0; i < sz; ++i)
    {
        pcb_t *p = (pcb_t *)list_at(&kernel.plist, i);
        if (p == kernel.current)
        {
            addr_list[idx++] = (pair){(DWORD)p->stack, p};
            addr_list[idx++] = (pair){(DWORD)get_esp(), p};
        }
        else
        {
            addr_list[idx++] = (pair){(DWORD)p->stack, p};
            addr_list[idx++] = (pair){p->cpu_state.SP.b32, p};
        }
    }

    for (size_t i = 0; i < idx - 1; ++i)
    for (size_t j = 0; j < idx - i - 1; ++j)
    if (addr_list[j].addr > addr_list[j+1].addr)
    {
        pair temp = addr_list[j];
        addr_list[j] = addr_list[j + 1];
        addr_list[j + 1] = temp;
    }

    for (size_t i = 0; i < idx; ++i)
    {
        pair *p = addr_list + i;
        printf("0x%lx: %s\n...\n", p->addr, p->owner->name);
    }

    free(addr_list);
}
