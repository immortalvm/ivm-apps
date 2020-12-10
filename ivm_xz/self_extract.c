#include "decompress.h"
#include "input.h"

/* Define DEBUG if run standalone. */
#ifdef DEBUG
#include <stdlib.h>
#endif

uint8_t *output;

int do_decompress(void) {
  /* Read compressed file from input and write it to output */
  return decompress(input, input_len, output, decompressed_size);
}

int main(void)
{
#ifdef DEBUG
  output = malloc(decompressed_size + 1);
#endif
  return do_decompress();
}
