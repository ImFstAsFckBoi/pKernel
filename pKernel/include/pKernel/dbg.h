#ifndef __CUSTOM_DBG_H
#define __CUSTOM_DBG_H

#include "stdio.h"
#include "pKernel.h"

extern pKernel kernel;
#define NAME kernel.current->name

#define STATIC_SIZE_PRINT(type) char(*__kaboom)[sizeof(type)] = 1;

#define OVERRIDE_ENABLE 1
#define OVERRIDE_VALUE 1

#ifndef DO_DEBUG
// ENABLE / DISABLE DEBUGGING
#define DO_DEBUG 0
#endif

#if OVERRIDE_ENABLE
#undef DO_DEBUG
#define DO_DEBUG OVERRIDE_VALUE
#endif


#if DO_DEBUG 
#define dbg_location printf("[DEBUG] [%s:%d] (%s)\n", __FILE__, __LINE__, NAME)

#define dbg_print(format) printf("[DEBUG] [%s:%d] (%s) "format, __FILE__, __LINE__, NAME)
#define dbg_printf(format, ...) printf("[DEBUG] [%s:%d] (%s) "format, __FILE__, __LINE__, NAME, __VA_ARGS__)
#define dbg_dump(fmt, x)   printf("[DEBUG] [%s:%d] (%s) Variable %s = "fmt"\n", __FILE__, __LINE__, NAME, #x, x)
#define dbg_dump_i(x)   printf("[DEBUG] [%s:%d] (%s) Variable %s = %d\n", __FILE__, __LINE__, NAME, #x, x)
#define dbg_dump_f(x)   printf("[DEBUG] [%s:%d] (%s) Variable %s = %f\n", __FILE__, __LINE__, NAME, #x, x)
#define dbg_dump_str(x) printf("[DEBUG] [%s:%d] (%s) Variable %s = %s\n", __FILE__, __LINE__, NAME, #x, x)
#define dbg_dump_ptr(x) printf("[DEBUG] [%s:%d] (%s) Variable %s = %p\n", __FILE__, __LINE__, NAME, #x, x)

#else

#define dbg_print(format) ;
#define dbg_printf(format, ...) ;
#define dbg_location ;
#define dbg_dump_i(x) ;
#define dbg_dump_f(x) ;
#define dbg_dump_str(x) ;
#define dbg_dump_ptr(x) ;

#endif


#endif
