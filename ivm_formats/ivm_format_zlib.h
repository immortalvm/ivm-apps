#ifndef IVM_FILE_FORMAT_ZLIB
#define IVM_FILE_FORMAT_ZLIB

#include "gvector.h"

struct ivm_file_format_s;
struct afs_toc_file_s;

int ivm_file_format_zlib_decode(struct ivm_file_format_s* format, struct afs_toc_file_s* file, gvector* in, gvector* out);

#endif
