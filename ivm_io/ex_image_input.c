/* Demonstrate use of the image input device. */

#include "ivm_io.h"
#include <stdio.h>

int main()
{
  long width = -1, height = -1;

  ivm64_read_frame (&width, &height);

  printf ("ivm64_read_frame: width=%ld, height=%ld\n", width, height);

  long v;
  

  puts ("ivm64_read_pixel:");
  
  for (long j = 0; j < height; j++) 
    {
      for (long i = 0; i < width; i++) 
        {
          v = ivm64_read_pixel (i, j);
          printf ("(%2ld,%2ld)=%3ld;", i, j, v);
        }
      puts ("");
    }
  
  return 0;
}
