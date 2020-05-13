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

void ivm64_read_frame(long number, long* widthp, long* heightp)
{
  long width = 0, height = 0;
  /* Could we avoid this intermediate storage,
     and instead write directly to the args in the asm below? */

#ifdef __ivm64__
  /* Pushes 16 bytes on the stack (first width, then height): */
  asm volatile("read_frame! (load8 %[n])" : : [n] "m" (number));
  /* Pops 8 bytes form the stack: */
  asm volatile("store8! (+ 16 %[h])" : [h] "=m" (height) : );
  /* Idem: */
  asm volatile("store8! (+  8 %[w])" : [w] "=m" (width) : );
#else
  printf ("read_frame ...");
  printf ("<store8-ed values: width=%d, height=%d>\n", width, height);
#endif

  *widthp = width;
  *heightp = height;
}

long ivm64_read_pixel(long x, long y)
{
  long v = 0;

#ifdef __ivm64__
  asm volatile("read_pixel* [(load8 %[x]) (load8 %[y])]"
               : : [x] "m" (x), [y] "m" (y) ); /* Pushes 8 bytes on the stack */
  asm volatile("store8! (+ 8 %[v])"
               : [v] "=m" (v) : ); /* Pops 8 bytes */
#else
  printf ("read_pixel* [%d %d])\n", x, y);
  printf ("<result value on stack: %d>\n", v);
#endif

  return v;
}
