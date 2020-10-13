#ifndef TRIVLIB_H
#define TRIVLIB_H

#include <stddef.h>
#include <stdint.h>

void *trivial_malloc (size_t size);
void trivial_free (void *ptr);

int trivial_memcmp(const void *m1, const void *m2, size_t n);
void *trivial_memcpy(void *dst0, const void *src0, size_t len0);
void *trivial_memmove(void *dst_void, const void *src_void, size_t length);
void *trivial_memset(void *m, int c, size_t n);
#endif
