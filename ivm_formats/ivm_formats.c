#include "ivm_formats.h"

#include "ivm_format_jpeg.h"
#include "ivm_format_tiff.h"
#include "ivm_format_zlib.h"
#include "ivm_format_pdf.h"

#include <string.h>

static ivm_file_format ivm_file_formats[] =
{
  { "afs/jpeg", ivm_file_format_jpeg_render, NULL },
  { "afs/tiff", ivm_file_format_tiff_render, NULL },
  //  { "afs/pdf",  ivm_file_format_pdf_render, NULL },
  { "ZLIB", NULL, ivm_file_format_zlib_decode },
};

unsigned ivm_file_format_count()
{
  return sizeof(ivm_file_formats) / sizeof(ivm_file_formats[0]);
}

ivm_file_format* ivm_file_format_get(unsigned index)
{
  return &ivm_file_formats[index];
}

ivm_file_format* ivm_file_format_find(char * name)
{
  for (unsigned i = 0; i < ivm_file_format_count(); i++) {
    ivm_file_format* format = ivm_file_format_get( i );
    if ( strcmp(format->file_format, name) == 0 ) {
      return format;
    }
  }

  return NULL;
}

