#ifndef IVM__IO_H
#define IVM__IO_H

void ivm64_new_frame(long width, long height, long rate);

void ivm64_set_pixel(long x, long y, long r, long g, long b);

void ivm64_read_frame(long number, long* width, long* height);

long ivm64_read_pixel(long x, long y);

#endif
