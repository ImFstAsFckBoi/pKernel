#include "pKernel/pKernel.h"
#include "pKernel/syscall.h"
#include "pKernel/time.h"
#include "stdio.h"

double calc_t_score(pcb_t *p);
extern pKernel kernel;
int clock_test_s()
{
    secs_t start = time_secs();
    for (;;)
    {
        printf("\n\n%04.0f: %d: Time since start: %lu s", calc_t_score(kernel.current), kernel.current->status, time_secs() - start);
        puts("\0338");
        fflush(stdout);
        pk_sleep(1000);
        pk_yield();
    }

    return 0;
}

int clock_test_ms()
{
    secs_t start = time_msecs();
    for (;;)
    {
        printf("\n%04.0f: %d: Time since start: %lu ms\r", calc_t_score(kernel.current), kernel.current->status, time_msecs() - start);
        puts("\0338");
        fflush(stdout);
        pk_sleep(1);
        pk_yield();
    }

    return 0;
}

int clock_test_us()
{
    secs_t start = time_usecs();
    for (;;)
    {
        printf("%04.0f: %d: Time since start: %lu us\r", calc_t_score(kernel.current), kernel.current->status, time_usecs() - start);
        fflush(stdout);
        pk_yield();
    }

    return 0;
}

void test_clock()
{
    puts("\0337");
    time_init(4500);

    pk_add_proc(clock_test_s, "Clock s", NULLARG);
    pk_add_proc(clock_test_ms, "Clock ms", NULLARG);
    pk_add_proc(clock_test_us, "Clock us", NULLARG);

}
