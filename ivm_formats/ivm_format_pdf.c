#include "ivm_format_pdf.h"

#include <stdio.h>
#include <gslib.h>
#include "ivm_io/ivm_io.h"
#include "ivm_formats/ivm_formats.h"
#include "afs/tocfile.h"

// https://www.ghostscript.com/doc/current/API.htm#add_fs

int func()
{
    int code, code1;
    const char * gsargv[7];
    int gsargc;
    gsargv[0] = "";
    gsargv[1] = "-dNOPAUSE";
    gsargv[2] = "-dBATCH";
    gsargv[3] = "-dSAFER";
    gsargv[4] = "-sDEVICE=pdfwrite";
    gsargv[5] = "-sOutputFile=out.pdf";
    gsargv[6] = "input.ps";
    gsargc=7;

    code = gsapi_new_instance(&minst, NULL);
    if (code < 0)
        return 1;
    code = gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8);
    if (code == 0)
        code = gsapi_init_with_args(minst, gsargc, gsargv);
    code1 = gsapi_exit(minst);
    if ((code == 0) || (code == gs_error_Quit))
        code = code1;

    gsapi_delete_instance(minst);

    if ((code == 0) || (code == gs_error_Quit))
        return 0;
    return 1;
}


int ivm_file_format_pdf_render(struct ivm_file_format* format, struct afs_toc_file* toc_file, gvector* data)
{
  int gs_lib_init(gp_file * debug_out);

  /* Clean up after execution. */
  gs_lib_finit(0, 0, NULL);


    
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
