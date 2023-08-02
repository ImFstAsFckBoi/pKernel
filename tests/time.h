#include "pKernel/pKernel.h"
#include "pKernel/syscall.h"
#include "pKernel/time.h"

int short_time()
{
    msecs_t start = time_msecs();
    pk_deadline_set(1000);
    printf("Short time start = %ld\n", start);
    pk_sleep(500);
    msecs_t t = time_msecs();
    printf("Short time end = %ld\n", t);
    printf("Short diff = %ld\n", t - start);
    int d = pk_deadline_meet();
    printf("Did short meet deadline?: %d\n", d);
    return 0;
}

int time_test()
{
    pk_deadline_set(500);

    msecs_t start = time_msecs();
    printf("Long time start = %ld\n", start);
    pk_exec(short_time, NULLARG);
    pk_sleep(1000);
    msecs_t t = time_msecs();
    printf("Long time end = %ld\n", t);
    printf("Long diff = %ld\n", t - start);
    int d = pk_deadline_meet();
    printf("Did long meet deadline?: %d\n", d);

    return 0;
}

void test_time()
{
    time_init(3700);
    pk_add_proc(time_test, "Time test", NULLARG);

}