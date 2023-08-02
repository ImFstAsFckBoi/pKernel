#ifndef SYSCALL_H_
#define SYSCALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "time.h"
/**
 * @brief Relinquish the CPU and schedule the next proccess.
 * 
 */
void pk_yield(void);
/**
 * @brief Wait atleast 'msec' milliseconds before continuing.
 * No guarantee that it won't wait longer as the running
 * process would need to yield in time. This is a form of busy wait.
 * 
 * @param msec Milliseconds to wait before allowing process to continue.
 */
void pk_sleep(msecs_t msec);
/**
 * @brief Lets a proccess add a subprocess to be executed.
 * 
 * @param entry_func Entry (or main) function of the subprocess.
 * @param args Arguments to be passed.
 * @return pid_t An ID which can be used to identify the subprocess when calling pk_wait().
 */
pid_t pk_exec(proc_func_t entry_func, arg_t args);
/**
 * @brief Wait for a subprocess to finish executing and get its exit code.
 * 
 * @param pid ID of the subprocess to wait for.
 * @return int The exit code of the subprocess.
 */
int pk_wait(pid_t pid);
/**
 * @brief Set a deadline for 'msecs' milliseconds in the future.
 * 
 * @param msecs Milliseconds from non the deadline needs to be met by.
 * @return int 0 = success, 1 = failure
 */
int pk_deadline_set(msecs_t msec);
/**
 * @brief Meet the deadline previously set by 'pk_deadline_set'
 * 
 * @return int 0 = deadline met, 1 = deadline exceeded
 */
int pk_deadline_meet();

#ifdef __cplusplus
}
#endif

#endif // SYSCALL_H_
