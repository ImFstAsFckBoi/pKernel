#ifndef TIME_H_
#define TIME_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD usecs_t;
typedef DWORD msecs_t;
typedef DWORD  secs_t;

/**
 * @brief Initialize time module. Must be run before
 * any other time function to not get incorrect values.
 * 
 * @param cpu_clk_mhz The clock speed of your CPU in MHz
 */
void time_init(size_t cpu_clk_mhz);
/**
 * @brief Get the x86 Time Stamp Counter. (rdtsc)
 * 
 */
QWORD timestamp(void);
/**
 * @brief Approximate the CPU running time in microseconds.
 * 
 * @return DWORD Microseconds the CPU has been running.
 */
usecs_t time_usecs(void);
/**
 * @brief Approximate the CPU running time in milliseconds.
 * 
 * @return DWORD Milliseconds the CPU has been running.
 */
msecs_t time_msecs(void);
/**
 * @brief Approximate the CPU running time in seconds.
 * 
 * @return DWORD Seconds the CPU has been running.
 */
secs_t time_secs(void);
/**
 * @brief Busy wait the CPU for msec amount of time.
 * 
 */
void busy_sleep(msecs_t msec);

#ifdef __cplusplus
}
#endif

#endif // TIME_H_
