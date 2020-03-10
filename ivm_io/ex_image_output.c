/* Demonstrate use of the image output device. */

#include "ivm_io.h"

int main()
{
  ivm64_new_frame (4, 2, 0);

  ivm64_set_pixel (0, 0, 255, 0, 0);
  ivm64_set_pixel (1, 0, 0, 255, 0);
  ivm64_set_pixel (1, 1, 0, 0, 255);
  
  return 0;
}
