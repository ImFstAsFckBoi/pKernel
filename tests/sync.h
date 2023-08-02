#include "pKernel/syscall.h"
#include "pKernel/sync.h"

sema s;

int sync_sub_test1(int argc, char **argv)
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

int sync_sub_test2(int argc, char **argv)
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

int sync_test()
{
    char *argv1[2] = {"45", "24"};
    arg_t arg1 = {2, argv1};
    pid_t pid1 = pk_exec(sync_sub_test1, arg1);
    
    char *argv2[2] = {"210", "210"};
    arg_t arg2 = {2, argv2};
    pid_t pid2 = pk_exec(sync_sub_test2, arg2);
    

    printf("F: This will be printed first\n");

    int code1 = pk_wait(pid1);
    printf("F: Subprocess 1 exited with code %d\n", code1);

    int code2 = pk_wait(pid2);
    printf("F: Subprocess 2 exited with code %d\n", code2);
    printf("F: This will be printed last\n");
    return 0;
}


void test_sync()
{
    sema_init(&s, 1);
    pk_add_proc(sync_test, "Sync test", NULLARG);

}