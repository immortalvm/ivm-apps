/* Demonstrate use of the image output device. */

#ifdef __ivm64__
#include <stdio.h>
#endif

void ivm64_new_frame(int width, int height, int rate)
{
#ifdef __ivm64__
  asm volatile("new_frame* [(load8 %0) (load8 %1) (load8 %2)]"
               ::"m"(width), "m"(height), "m"(rate));
#else
  printf ("new_frame* [%d %d %d]\n", width, height, rate);
#endif
}

void ivm64_set_pixel(int x, int y, int r, int g, int b) 
{
#ifdef __ivm64__
  asm volatile("set_pixel* [(load8 %0) (load8 %1) (load8 %2) (load8 %3) (load8 %4)]"
               ::"m"(x), "m"(y), "m"(r), "m"(g), "m"(b));
#else
  printf ("set_pixel* [%d %d %d %d %d])\n", x, y, r, g, b);
#endif
}


int main()
{
  ivm64_new_frame (4, 2, 0);

  ivm64_set_pixel (0, 0, 255, 0, 0);
  ivm64_set_pixel (1, 0, 0, 255, 0);
  ivm64_set_pixel (1, 1, 0, 0, 255);
  
  return 0;
}
