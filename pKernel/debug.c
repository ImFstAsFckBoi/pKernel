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
