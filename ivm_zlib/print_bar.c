#include <stdio.h>

void printbar() 
{
  puts("bar");
#ifdef __ivm64__
  asm volatile("exit! 0");
#else
  #error "ERROR: This code only makes sense for ivm64"
#endif

}
