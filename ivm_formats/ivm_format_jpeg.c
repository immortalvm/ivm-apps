#include "ivm_format_jpeg.h"

#include <stdio.h>
#include <jpeglib.h>
#include "ivm_io/ivm_io.h"
#include "ivm_formats/ivm_formats.h"
#include "afs/tocfile.h"
#include <setjmp.h>

// Code based on jpeg lib example

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

static const char* jpeg_color_space_name( J_COLOR_SPACE cs );
static void my_error_exit (j_common_ptr cinfo);

int ivm_file_format_jpeg_render(ivm_file_format* format, afs_toc_file* toc_file, gvector* data)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    printf( "JPEG decode exception\n" );
    return 1;
  }

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
  ivm64_new_frame( cinfo.image_width, cinfo.image_height, 0 );
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    JSAMPROW s = buffer[0];
    for ( unsigned x = 0; x < row_stride; x++ ) {
      ivm64_set_pixel(x, y, s[0], s[1], s[2]);
      s += 3;
    }
    y++;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  
  return 0;
}

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

/*
 * Here's the routine that will replace the standard error_exit method:
 */

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}
