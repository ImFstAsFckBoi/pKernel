#ifndef SYSCALL_H_
#define SYSCALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "time.h"
/**
 * @brief Relinquish the CPU schedule the next proccess.
 * 
 */
void pk_yield(void);
/**
 * @brief Wait atleast msec milliseconds until continuing.
 * No guarantee that that it won't wait longer as running
 * process would need to yield. This is a form of busy wait.
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
 * @brief 
 * 
 * @param msecs 
 * @return int 
 */
int pk_deadline_set(msecs_t msec);
/**
 * @brief 
 * 
 * @return int 
 */
int pk_deadline_meet();

#ifdef __cplusplus
}
#endif

#endif // SYSCALL_H_
