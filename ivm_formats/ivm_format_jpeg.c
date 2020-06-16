#include "ivm_format_jpeg.h"

#include <stdio.h>
#include <jpeglib.h>
#include "ivm_io/ivm_io.h"
#include "ivm_formats/ivm_formats.h"
#include "tocfile.h"


static const char* jpeg_color_space_name( J_COLOR_SPACE cs )
{
  switch (cs) {
    case JCS_UNKNOWN : return "unknown";
    case JCS_GRAYSCALE: return "grayscale";
    case JCS_RGB: return "sRGB";
    case JCS_YCbCr: return "YUV";
    case JCS_CMYK: return "CMYK";
    case JCS_YCCK: return "YCCK";
    case JCS_BG_RGB: return "big gamut RGB";
    case JCS_BG_YCC: return "big gamut YUV";
  }
  return "undefined";
}

int ivm_file_format_jpeg_render(ivm_file_format* format, afs_toc_file* toc_file, gvector* data)
{
  struct jpeg_decompress_struct cinfo;

  jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo, data->buffer, data->size);

  int res = jpeg_read_header(&cinfo, TRUE);
  if ( res != JPEG_HEADER_OK ) {
    printf( "JPEG header decode error\n" );
    return 1;
  }

  printf( "JPEG dimensions: %dx%d\n", cinfo.image_width, cinfo.image_height );
  printf( "JPEG components: %d\n", cinfo.num_components );
  printf( "JPEG color-space: %s\n", jpeg_color_space_name(cinfo.out_color_space) );

  cinfo.out_color_space = JCS_RGB;
  
  boolean ok = jpeg_start_decompress(&cinfo);
  if (!ok) {
    printf( "JPEG feild to start decompression\n" );
    return 1;
  }
  
  int row_stride = cinfo.output_width * cinfo.output_components;

  JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

  unsigned y = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    for ( unsigned x = 0; x < row_stride; x++ ) {
      ivm64_set_pixel(x, y, *(buffer + 0), *(buffer + 1), *(buffer + 2));
      buffer += 3;
    }
    y++;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  
  return 0;
}
