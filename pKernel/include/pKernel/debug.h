#ifndef DEBUG_H_
#define DEBUG_H_

#include "types.h"
#include "pKernel.h"

#ifdef __cplusplus
extern "C" {
#endif

// Force GCC to not strip these functions
// so they can be used in GDB when debugging
// without being used anywhere in the code
asm(".GLOBAL get_esp        \n"
    ".GLOBAL get_sp         \n"
    ".GLOBAL print_stack    \n"
    ".GLOBAL stack_top      \n"
    ".GLOBAL where_is_stack \n");


DWORD *get_esp();

BYTE *get_sp();

void print_stack(pcb_t *p, size_t group);

DWORD stack_top();

char *where_is_stack(void);

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H_
