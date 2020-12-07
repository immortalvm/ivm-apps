#include "uncompress.h"
#include "input.h"
#include "output.h"

void do_uncompress(void) {
  /* Read compressed file from input and write it to output */
  uncompress(input, input_len, output, sizeof(output));
}

int main(void)
{
  do_uncompress();
  return 0;
}
