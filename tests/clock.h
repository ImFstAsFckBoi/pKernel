#include "pKernel/pKernel.h"
#include "pKernel/syscall.h"
#include "pKernel/time.h"
#include "stdio.h"
int clock_test_s()
{
    secs_t start = time_secs();
    for (;;)
    {
        printf("\n\nTime since start: %lu s", time_secs() - start);
        puts("\0338");
        fflush(stdout);
        pk_sleep(1000);
    }

    return 0;
}

int clock_test_ms()
{
    secs_t start = time_msecs();
    for (;;)
    {
        printf("\nTime since start: %lu ms\r", time_msecs() - start);
        puts("\0338");
        fflush(stdout);
        pk_sleep(1);
    }

    return 0;
}

int clock_test_us()
{
    secs_t start = time_usecs();
    for (;;)
    {
        printf("Time since start: %lu us\r", time_usecs() - start);
        fflush(stdout);
        pk_yield();
    }

    return 0;
}

void test_clock()
{
    //puts("\0337");
    time_init(4500);

    pk_add_proc(clock_test_s, "Clock s", NULLARG);
    pk_add_proc(clock_test_ms, "Clock ms", NULLARG);
    pk_add_proc(clock_test_us, "Clock us", NULLARG);

}
