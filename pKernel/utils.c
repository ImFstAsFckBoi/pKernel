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

void pk_panic(const char *msg)
{
    printf("KERNEL PANIC! with message:\n%s\n\nExiting pKernel and continuing main function", msg);
    pk_ctx_switch(&kernel.main_proc);
}

void pk_assert(bool cond, char *msg)
{
    if (cond)
        return;
    
    if (msg != NULL)
        printf("Assertion failed: %s\n", msg);

    pk_panic("Failed assertion.");
}

void swap(void *a, void *b)
{
    //X := X XOR Y; // XOR the values and store the result in X
    //Y := Y XOR X; // XOR the values and store the result in Y
    //X := X XOR Y; // XOR the values and store the result in X
    DWORD tmp = *(DWORD *)a;
    *(DWORD *)a = *(DWORD *)b;
    *(DWORD *)b = tmp;
}