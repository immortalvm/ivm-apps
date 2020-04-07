#include "ivm_io.h"

#ifdef __ivm64__
#include <assert.h>
static_assert(sizeof(long) == 8, "8 bytes in a long");
#else
#include <stdio.h>
#endif

void ivm64_new_frame(long width, long height, long rate)
{
#ifdef __ivm64__
  asm volatile("new_frame* [(load8 %[w]) (load8 %[h]) (load8 %[r])]"
               : : [w] "m" (width), [h] "m" (height), [r] "m" (rate));
#else
  printf ("new_frame* [%d %d %d]\n", width, height, rate);
#endif
}

void ivm64_set_pixel(long x, long y, long r, long g, long b) 
{
#ifdef __ivm64__
  asm volatile("set_pixel* [(load8 %[x]) (load8 %[y]) (load8 %[r]) (load8 %[g]) (load8 %[b])]"
               : : [x] "m" (x), [y] "m" (y), [r] "m" (r), [g] "m" (g), [b] "m" (b));
#else
  printf ("set_pixel* [%d %d %d %d %d])\n", x, y, r, g, b);
#endif
}
