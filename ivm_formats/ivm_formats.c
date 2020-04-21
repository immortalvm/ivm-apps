#include "ivm_formats.h"

#include "ivm_format_tiff.h"
#include "ivm_format_zlib.h"


static ivm_file_format ivm_file_formats[] =
{
  { "TIFF", ivm_file_format_tiff_render, NULL },
  { "ZLIB", NULL, ivm_file_format_zlib_decode }
};

unsigned ivm_file_format_count()
{
  return sizeof(ivm_file_formats) / sizeof(ivm_file_formats[0]);
}

ivm_file_format* ivm_file_format_get(unsigned index)
{
  return &ivm_file_formats[index];
}

