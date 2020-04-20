#ifndef IVM_FILE_FORMATS
#define IVM_FILE_FORMATS

#include "gvector.h"

struct ivm_file_format_s;
struct afs_toc_file_s;

typedef int (*ivm_file_format_render_cb)(struct ivm_file_format_s* format, struct afs_toc_file_s* file, gvector* in);
typedef int (*ivm_file_format_decode_cb)(struct ivm_file_format_s* format, struct afs_toc_file_s* file, gvector* in, gvector* out);

typedef struct ivm_file_format_s
{
  char * file_format;
  ivm_file_format_render_cb render;
  ivm_file_format_decode_cb decode;
} ivm_file_format;


unsigned         ivm_file_format_count();
ivm_file_format* ivm_file_format_get(unsigned index);

#endif
