#include <stdlib.h>
#include <stdio.h>


#include "pKernel/pKernel.h"
#include "pKernel/debug.h"
#include "pKernel/sync.h"
#include "pKernel/list.h"


int sub_test1(int argc, char **argv)
{
    int a = 0;

    for (int i = 0; i < argc; ++i)
    {
        a += atoi(argv[i]);
        pk_yield();
    }

    return a;
}

int sub_test2(int argc, char **argv)
{
    int a = 0;

    for (int i = 0; i < argc; ++i)
    {
        a += atoi(argv[i]);
    }

    return a;
}

int test1()
{
    char *argv1[2] = {"45", "24"};
    arg_t arg1 = {2, argv1};
    pid_t pid1 = pk_exec(sub_test1, arg1);
    
    char *argv2[2] = {"210", "210"};
    arg_t arg2 = {2, argv2};
    pid_t pid2 = pk_exec(sub_test2, arg2);
    

    printf("This will be printed first\n");

    int code1 = pk_wait(pid1);
    printf("Subprocess 1 exited with code %d\n", code1);

    int code2 = pk_wait(pid2);
    printf("Subprocess 2 exited with code %d\n", code2);

    return 0;
}


int main()
{
    pk_init(NULL);

    arg_t arg = {0, NULL};
    pk_add_proc(test1, "Main process", arg);

    pk_run();

    return 0;
}
