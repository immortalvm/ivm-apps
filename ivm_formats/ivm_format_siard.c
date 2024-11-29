#include "ivm_format_siard.h"

#include "afs/tocfile.h"
#include "ivm_formats/ivm_formats.h"
#include "ivm_io/ivm_io.h"

#include "../../ROAEshell/ida/siard2sql/siard2sql.h"

#include <stdio.h>
#include <string.h>

#define log(fmt, ...)                                                          \
  fprintf(stderr,                                                              \
          "[ivm_file_format_siard] " fmt "\n" __VA_OPT__(, ) __VA_ARGS__)

int ivm_file_format_siard_render(ivm_file_format *format, afs_toc_file *file,
                                 gvector *data) {
  log("render: %s", file->name);
  if (strcmp(file->name, "encoding.siard") != 0) {
    log("%s is not encoding.siard, skipping...", file->name);
    return 0;
  }
  if (!(file->size == data->size))
    log("SIZE MISMATCH: TOC: %zu, DATA: %zu", file->size, data->size);
  FILE *siard = fopen("blob.siard", "w+");
  size_t total_items_written = 0;
  while (total_items_written < data->size) {
    size_t items_written =
        fwrite(data->buffer, data->item_size, data->size, siard);
    if (items_written == 0) {
      log("Failed to write siard file to tmpfs");
      return 1;
    }
    total_items_written += items_written;
  }
  fclose(siard);
  log("Wrote %zu bytes", total_items_written);
  IDA_siard2sql("blob.siard", "blob.sql", "");
  remove("blob.siard");
  FILE *sql = fopen("blob.sql", "r");
  size_t bytes_read = 0;
  char buf[4096];
  while ((bytes_read = fread(buf, 1, 4096, sql))) {
    fprintf(stdout, "%.*s", bytes_read, buf);
  }
  return 0;
}

int ivm_file_format_siard_decode(ivm_file_format *format, afs_toc_file *file,
                                 gvector *in, gvector *out) {
  log("decode: %s", file->name);
  return 0;
}
