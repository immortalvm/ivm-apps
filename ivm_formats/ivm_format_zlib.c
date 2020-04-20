#include "ivm_format_zlib.h"

#include "zlib.h"

static int zlib_uncompress(const uint8_t* src, unsigned src_len, uint8_t* dest, unsigned dest_len);

int ivm_file_format_zlib_decode(struct ivm_file_format_s* format, struct afs_toc_file_s* file, gvector* in, gvector* out)
{
  return zlib_uncompress(in->buffer, in->size, out->buffer, out->size);
}


static int zlib_uncompress(const uint8_t* src, unsigned src_len, uint8_t* dest, unsigned dest_len)
{
    z_stream z_info  = { 0 };
    z_info.total_in  = z_info.avail_in  = src_len;
    z_info.total_out = z_info.avail_out = dest_len;
    z_info.next_in   = (uint8_t*)src;
    z_info.next_out  = dest;

    int err, ret = -1;
    err = inflateInit( &z_info );
    if ( err == Z_OK )
    {
        err = inflate( &z_info, Z_FINISH );
        if ( err == Z_STREAM_END )
        {
            ret = z_info.total_out;
        }
    }
    inflateEnd( &z_info );
    return ret ; // -1 or length of output
}
