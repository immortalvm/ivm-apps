#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "xz.h"

int decompress(char *in_buf, int in_buf_size, char *out_buf, int out_buf_size);

#endif
