/* Demonstrate use of the image input device. */

#include "ivm_io.h"
#include <stdio.h>

int main()
{
  long width = 333, height = 444;

  ivm64_read_frame (&width, &height, 0); /* TODO: generalize to n frames */

  printf ("ivm64_read_frame: width=%ld, height=%ld\n", width, height);

  if (width + height == 0) 
    {
      puts ("No frames found");
      return 1;
    }
  
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
