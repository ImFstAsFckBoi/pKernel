#include <stdlib.h>
#include <stdio.h>

#include "pKernel/pKernel.h"
#include "pKernel/debug.h"
#include "pKernel/sync.h"
#include "pKernel/list.h"

sema s;

int full_sub_test1(int argc, char **argv)
{
    int a = 0;
    pk_yield();
    sema_down(&s);
    for (int i = 0; i < argc; ++i)
    {
        a += atoi(argv[i]);
    }

    printf("F: This will be printed third\n");
    return a;
}

int full_sub_test2(int argc, char **argv)
{
    sema_down(&s);
    int a = 0;
    pk_yield();
    for (int i = 0; i < argc; ++i)
    {
        a += atoi(argv[i]);
    }

    pk_yield();

    printf("F: This will be printed second\n");
    sema_up(&s);
    return a;
}

int full_test()
{
    char *argv1[2] = {"45", "24"};
    arg_t arg1 = {2, argv1};
    pid_t pid1 = pk_exec(full_sub_test1, arg1);
    
    char *argv2[2] = {"210", "210"};
    arg_t arg2 = {2, argv2};
    pid_t pid2 = pk_exec(full_sub_test2, arg2);
    

    printf("F: This will be printed first\n");

    int code1 = pk_wait(pid1);
    printf("F: Subprocess 1 exited with code %d\n", code1);

    int code2 = pk_wait(pid2);
    printf("F: Subprocess 2 exited with code %d\n", code2);
    printf("F: This will be printed last\n");
    return 0;
}

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
    arg_t arg = {0, NULL};
    pk_exec(deadlock_sub, arg);
    printf("D: Main tried to lock s1\n");
    sema_down(&s1);
    printf("D: Main locked s1\n");
    pk_yield();
    printf("D: Main tried to lock s2\n");
    sema_down(&s2);
    printf("D: Main locked s2\n");
    return 0;
}

int main()
{
    sema_init(&s, 1);
    sema_init(&s1, 1);
    sema_init(&s2, 1);
    pk_init(NULL);

    arg_t arg = {0, NULL};
    pk_add_proc(full_test, "Full test", arg);
    pk_add_proc(deadlock_test, "Deadlock test", arg);

    pk_run();

    return 0;
}
