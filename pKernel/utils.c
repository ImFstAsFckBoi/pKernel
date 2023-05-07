#include "utils.h"
#include "pKernel.h"
#include "stdio.h"

extern pKernel kernel;


void *memcpy_rev(void *dest, void *src, size_t len)
{
    for (int i = len - 1; i >= 0; --i)
        ((BYTE *)dest)[i] = ((BYTE *)src)[i];

    return dest;
}

void pk_assert(bool cond, char *msg)
{
    if (cond)
        return;
    
    if (msg != NULL)
        printf("Assertion failed: %s\n Exiting pKernel and continuing main function\n", msg);

    pk_ctx_switch(&kernel.main_proc);
}
