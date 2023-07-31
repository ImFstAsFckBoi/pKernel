#include "time.h"

DWORD CPU_CLK_SCALE = 0;

void time_init(size_t cpu_clk_mhz)
{
    CPU_CLK_SCALE = cpu_clk_mhz;
}

usecs_t time_usecs(void)
{
    QWORD ts = timestamp();
    return ts / CPU_CLK_SCALE;
}

msecs_t time_msecs(void)
{
    QWORD ts = timestamp();
    return (ts / CPU_CLK_SCALE) / 1000;
}

secs_t time_secs(void)
{
    
    QWORD ts = timestamp();
    return (ts / CPU_CLK_SCALE) / 1000000;
}

void busy_sleep(msecs_t msec)
{
    msecs_t start = time_msecs();
    while (time_msecs() < start + msec)
        ;
}
