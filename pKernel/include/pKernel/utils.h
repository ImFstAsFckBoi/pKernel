#ifndef UTILS_H_
#define UTILS_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy_rev(void *dest, void *src, size_t len);

void pk_panic(const char *msg);

void pk_assert(bool cond, char *msg);

void swap(void *a, void *b);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H_

