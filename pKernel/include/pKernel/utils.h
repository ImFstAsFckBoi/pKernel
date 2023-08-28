#ifndef UTILS_H_
#define UTILS_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reversed memcpy
 * 
 * @param dest Destination address.
 * @param src Source address.
 * @param len Number of bytes to copy.
 * @return void* dest
 */
void *memcpy_rev(void *dest, void *src, size_t len);
/**
 * @brief Swap two 8 bit variables with the XOR swap algorithm
 * 
 * @param a 8 bit pointer to variable to be swapped to b.
 * @param b 8 bit pointer to variable to be swapped to a.
 */
void xorswap(BYTE *a, BYTE *b);
/**
 * @brief Swap two 8 bit variables.
 * 
 * @param a 8 bit pointer to variable to be swapped to b.
 * @param b 8 bit pointer to variable to be swapped to a.
 */
void swap(BYTE *a, BYTE *b);
/**
 * @brief Swap two 16 bit variables with the XOR swap algorithm
 * 
 * @param a 16 bit pointer to variable to be swapped to b.
 * @param b 16 bit pointer to variable to be swapped to a.
 */
void xorswapw(WORD *a, WORD *b);
/**
 * @brief Swap two 16 bit variables.
 * 
 * @param a 16 bit pointer to variable to be swapped to b.
 * @param b 16 bit pointer to variable to be swapped to a.
 */
void swapw(WORD *a, WORD *b);
/**
 * @brief Swap two 32 bit variables with the XOR swap algorithm
 * 
 * @param a 32 bit pointer to variable to be swapped to b.
 * @param b 32 bit pointer to variable to be swapped to a.
 */
void xorswapl(DWORD *a, DWORD *b);
/**
 * @brief Swap two 32 bit variables.
 * 
 * @param a 32 bit pointer to variable to be swapped to b.
 * @param b 32 bit pointer to variable to be swapped to a.
 */
void swapl(DWORD *a, DWORD *b);
/**
 * @brief Swap two 64 bit variables with the XOR swap algorithm
 * 
 * @param a 64 bit pointer to variable to be swapped to b.
 * @param b 64 bit pointer to variable to be swapped to a.
 */
void xorswapq(QWORD *a, QWORD *b);
/**
 * @brief Swap two 64 bit variables.
 * 
 * @param a 64 bit pointer to variable to be swapped to b.
 * @param b 64 bit pointer to variable to be swapped to a.
 */
void swapq(QWORD *a, QWORD *b);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H_

