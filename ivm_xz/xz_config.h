/*
 * Private includes and definitions for userspace use of XZ Embedded
 *
 * Author: Lasse Collin <lasse.collin@tukaani.org>
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

#ifndef XZ_CONFIG_H
#define XZ_CONFIG_H

/* Uncomment as needed to enable BCJ filter decoders. */
/* #define XZ_DEC_X86 */
/* #define XZ_DEC_POWERPC */
/* #define XZ_DEC_IA64 */
/* #define XZ_DEC_ARM */
/* #define XZ_DEC_ARMTHUMB */
/* #define XZ_DEC_SPARC */

#include <stdbool.h>
#include "trivlib.h"
#include "xz.h"

/* Rewire C library calls to trivial functions */
#define malloc(size) trivial_malloc(size)
#define free(ptr) trivial_free(ptr)
#define memcmp(m1, m2, n) trivial_memcmp(m1, m2, n)
#define memcpy(dst, src, len) trivial_memcpy(dst, src, len)
#define memmove(dst, src, len) trivial_memmove(dst, src, len)
#define memset(m, c, n) trivial_memset(m, c, n)

#define memeq(a, b, size) (memcmp(a, b, size) == 0)
#define memzero(buf, size) memset(buf, 0, size)

#define kmalloc(size, flags) malloc(size)
#define kfree(ptr) free(ptr)
#define vmalloc(size) malloc(size)
#define vfree(ptr) free(ptr)

#ifndef min
#	define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#define min_t(type, x, y) min(x, y)

/*
 * Some functions have been marked with __always_inline to keep the
 * performance reasonable even when the compiler is optimizing for
 * small code size. You may be able to save a few bytes by #defining
 * __always_inline to plain inline, but don't complain if the code
 * becomes slow.
 *
 * NOTE: System headers on GNU/Linux may #define this macro already,
 * so if you want to change it, you need to #undef it first.
 */
#undef __always_inline
#define __always_inline inline

/* Inline functions to access unaligned unsigned 32-bit integers */
#ifndef get_unaligned_le32
static inline uint32_t get_unaligned_le32(const uint8_t *buf)
{
	return (uint32_t)buf[0]
			| ((uint32_t)buf[1] << 8)
			| ((uint32_t)buf[2] << 16)
			| ((uint32_t)buf[3] << 24);
}
#endif

#ifndef get_unaligned_be32
static inline uint32_t get_unaligned_be32(const uint8_t *buf)
{
	return (uint32_t)(buf[0] << 24)
			| ((uint32_t)buf[1] << 16)
			| ((uint32_t)buf[2] << 8)
			| (uint32_t)buf[3];
}
#endif

#ifndef put_unaligned_le32
static inline void put_unaligned_le32(uint32_t val, uint8_t *buf)
{
	buf[0] = (uint8_t)val;
	buf[1] = (uint8_t)(val >> 8);
	buf[2] = (uint8_t)(val >> 16);
	buf[3] = (uint8_t)(val >> 24);
}
#endif

#ifndef put_unaligned_be32
static inline void put_unaligned_be32(uint32_t val, uint8_t *buf)
{
	buf[0] = (uint8_t)(val >> 24);
	buf[1] = (uint8_t)(val >> 16);
	buf[2] = (uint8_t)(val >> 8);
	buf[3] = (uint8_t)val;
}
#endif

/*
 * Use get_unaligned_le32() also for aligned access for simplicity. On
 * little endian systems, #define get_le32(ptr) (*(const uint32_t *)(ptr))
 * could save a few bytes in code size.
 */
#ifndef get_le32
#	define get_le32 get_unaligned_le32
#endif

#endif
