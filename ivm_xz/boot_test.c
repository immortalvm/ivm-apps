#include "uncompress.h"
#include "input.h"

#define BUF_SIZE (8 * 1024 * 1024)

uint8_t output[BUF_SIZE];

int do_uncompress(void) {
  /* Read compressed file from input and write it to output */
  return uncompress(input, input_len, output, BUF_SIZE);
}

int main(void)
{
  return do_uncompress();
}
