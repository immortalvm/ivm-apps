#ifndef IVM_FILE_FORMAT_JPEG
#define IVM_FILE_FORMAT_JPEG

#include "gvector.h"

struct ivm_file_format_s;
struct afs_toc_file_s;

int ivm_file_format_jpeg_render(struct ivm_file_format_s* format, struct afs_toc_file_s* file, gvector* data);

#endif
