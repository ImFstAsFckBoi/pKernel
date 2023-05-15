#ifndef TIME_H_
#define TIME_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize time module. Must be run before
 * any other time function to not get incorrect values.
 * 
 * @param cpu_clk_mhz The clock speed of your CPU in MHz
 */
void time_init(size_t cpu_clk_mhz);
/**
 * @brief Get the x86 Time Stamp Counter.
 * 
 */
QWORD timestamp(void);
/**
 * @brief Approximate the CPU running time in microseconds.
 * 
 * @return DWORD Microseconds the CPU has been running.
 */
DWORD usecs(void);
/**
 * @brief Approximate the CPU running time in milliseconds.
 * 
 * @return DWORD Milliseconds the CPU has been running.
 */
DWORD msecs(void);
/**
 * @brief Approximate the CPU running time in seconds.
 * 
 * @return DWORD Seconds the CPU has been running.
 */
DWORD secs(void);

#ifdef __cplusplus
}
#endif

#endif // TIME_H_
