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

void xorswap(BYTE *a, BYTE *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

void swap(BYTE *a, BYTE *b)
{
    BYTE tmp = *a;
    *a = *b;
    *b = tmp;
}

void xorswapw(WORD *a, WORD *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

void swapw(WORD *a, WORD *b)
{
    WORD tmp = *a;
    *a = *b;
    *b = tmp;
}

void xorswapl(DWORD *a, DWORD *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

void swapl(DWORD *a, DWORD *b)
{
    DWORD tmp = *a;
    *a = *b;
    *b = tmp;
}

void xorswapq(QWORD *a, QWORD *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

void swapq(QWORD *a, QWORD *b)
{
    QWORD tmp = *a;
    *a = *b;
    *b = tmp;
}
