#include "time.h"

DWORD CPU_CLK_SCALE = 0;

void time_init(size_t cpu_clk_mhz)
{
    CPU_CLK_SCALE = cpu_clk_mhz;
}

DWORD usecs(void)
{
    QWORD ts = timestamp();
    return ts / CPU_CLK_SCALE;
}

DWORD msecs(void)
{
    QWORD ts = timestamp();
    return (ts / CPU_CLK_SCALE) / 1000;
}

DWORD secs(void)
{
    
    QWORD ts = timestamp();
    return (ts / CPU_CLK_SCALE) / 1000000;
}
