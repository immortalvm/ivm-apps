#include "uncompress.h"
#include "input.h"

#define BUFFER_SIZE (1024 * 1024)

void ivm64_put_character(long character)
{
  asm volatile("put_char* [(load8 %[c])]" : : [c] "m" (character));
}

int main(void)
{
  uint8_t out[BUFFER_SIZE];
  int position;

  position = uncompress(in, in_len, out, sizeof(out));
  if (position) {
    for (int i = 0; i < position; i++) {
      ivm64_put_character((long) out[i]);
    }
  }
  return 0;
}
