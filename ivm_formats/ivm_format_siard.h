#ifndef IVM_FILE_FORMAT_SIARD
#define IVM_FILE_FORMAT_SIARD

#include "gvector.h"

struct ivm_file_format_s;
struct afs_toc_file_s;

int ivm_file_format_siard_render(struct ivm_file_format_s *format,
                                 struct afs_toc_file_s *file, gvector *data);
int ivm_file_format_siard_decode(struct ivm_file_format_s *format,
                                 struct afs_toc_file_s *file, gvector *in,
                                 gvector *out);

#endif // IVM_FILE_FORMAT_SIARD
