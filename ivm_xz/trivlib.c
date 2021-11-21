#include "trivlib.h"

/* Trivial versions of malloc and free */

#if OLD_HEAP_NAME
extern uint64_t __heap_start; // Name of heap pointer can change based on compiler version
#else
extern uint64_t __IVM64_heap_pointer__;
#endif

uint64_t next_free = 0;

void *trivial_malloc (size_t size) {
  uint64_t tmp;

  if (next_free == 0) {
#if OLD_HEAP_NAME
    next_free = __heap_start;
#else
    next_free = __IVM64_heap_pointer__;
#endif
  }
  tmp = next_free;
  next_free += size;
  return (void *) tmp;
}

void trivial_free (void *ptr) {
}

/* A set of very simple mem* operations */

int trivial_memcmp(const void *m1, const void *m2, size_t n) {
  unsigned char *s1 = (unsigned char *) m1;
  unsigned char *s2 = (unsigned char *) m2;

  while (n--) {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
  }
  return 0;
}

void *trivial_memcpy(void *dst0, const void *src0, size_t len0) {
  char *dst = (char *) dst0;
  char *src = (char *) src0;
  void *save = dst0;

  while (len0--) {
    *dst++ = *src++;
  }
  return save;
}

void *trivial_memmove(void *dst_void, const void *src_void, size_t length) {
  char *dst = dst_void;
  const char *src = src_void;

  if (src < dst && dst < src + length) {
    src += length;
    dst += length;
    while (length--) {
      *--dst = *--src;
    }
  } else {
    while (length--) {
      *dst++ = *src++;
    }
  }
  return dst_void;
}

void *trivial_memset(void *m, int c, size_t n) {
  char *s = (char *) m;

  while (n--)
    *s++ = (char) c;
  return m;
}
