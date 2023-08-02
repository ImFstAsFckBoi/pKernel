#include "pKernel/pKernel.h"
#include "pKernel/syscall.h"
#include "pKernel/sync.h"
#include "stdio.h"

sema s1, s2;

int deadlock_sub()
{
    printf("D: Entering deadlock sub\n");
    printf("D: Sub tried to lock s2\n");
    sema_down(&s2);
    printf("D: Sub locked s2\n");
    pk_yield();
    printf("D: Sub tried to lock s1\n");
    sema_down(&s1);
    printf("D: Sub locked s1\n");
    return 0;
}

int deadlock_test()
{
    printf("D: Entering deadlock main\n");
    pk_exec(deadlock_sub, NULLARG);
    printf("D: Main tried to lock s1\n");
    sema_down(&s1);
    printf("D: Main locked s1\n");
    pk_yield();
    printf("D: Main tried to lock s2\n");
    sema_down(&s2);
    printf("D: Main locked s2\n");
    return 0;
}

void test_deadlock()
{
    sema_init(&s1, 1);
    sema_init(&s2, 1);
    pk_add_proc(deadlock_test, "Deadlock test", NULLARG);

}