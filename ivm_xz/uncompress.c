#include "uncompress.h"

int uncompress(char *in_buf, int in_buf_size, char *out_buf, int out_buf_size) {
  struct xz_buf b;
  struct xz_dec *s;

  xz_crc32_init();
  s = xz_dec_init(XZ_SINGLE, 0);
  if (s == NULL) {
    return 0;
  }
  b.in = in_buf;
  b.in_size = in_buf_size;
  b.in_pos = 0;
  b.out = out_buf;
  b.out_size = out_buf_size;
  b.out_pos = 0;
  xz_dec_run(s, &b);
  xz_dec_end(s);
  return b.out_pos;
}
