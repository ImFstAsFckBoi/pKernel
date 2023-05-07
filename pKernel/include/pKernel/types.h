#ifndef TYPES_H_
#define TYPES_H_

#include "stddef.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NO_DCE __attribute__((used))
#define NO_RET __attribute__((noreturn))


typedef unsigned long long  QWORD;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;

//typedef unsigned long       size_t;
//typedef int                 bool;

typedef int (*proc_func_t)(int argc, char **argv);

//#define true                1;
//#define false               0;

typedef int pid_t;

typedef int err_t;

typedef enum ERROR_CODES
{
    ERR_SUCCESS,
    ERR_FAILED
} ERROR_CODES;

typedef struct arg_t
{
    int argc;
    char **argv;
} arg_t;

#ifdef __cplusplus
}
#endif

#endif // TYPES_H_
