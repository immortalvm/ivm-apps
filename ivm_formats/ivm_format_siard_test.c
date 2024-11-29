#include "ivm_format_siard.c"

#include "../../boxing/inc/boxing/log.h"

#include "siard/simpledb.h"

#include <stdarg.h>
#include <stdio.h>

const char *log_level_str(int log_level) {
  switch (log_level) {
  case DLOG_LEVEL_INFO:
    return "\x1b[96mINFO\x1b[0m";
  case DLOG_LEVEL_WARNING:
    return "\x1b[93mWARNING\x1b[0m";
  case DLOG_LEVEL_ERROR:
    return "\x1b[91mERROR\x1b[0m";
  case DLOG_LEVEL_FATAL:
    return "\x1b[91mFATAL\x1b[0m";
  default:
    return "\x1b[95mUNKNOWN\x1b[0m";
  }
}

void boxing_log(int log_level, const char *string) {
  printf("%s: %s\n", log_level_str(log_level), string);
}

void boxing_log_args(int log_level, const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("%s: ", log_level_str(log_level));
  vprintf(format, args);
  printf("\n");

  va_end(args);
}

int main(void) {
  fprintf(stdout, "Hello, %s!\n", "World");
  afs_toc_file file = {
      .name = "simpledb.siard",
      .date = "2024-11-14 10:15:20",
      .checksum = "a9df548a866ec713d521c90e0087c0d8d0fd7648",
      .size = 17373,
      .id = 16,
      .start_frame = 32,
      .start_byte = 564566,
      .end_frame = 32,
      .end_byte = 581938,
      .unique_id = "D01D158B-B6C1-4679-AAF0-60E4B29D5296",
      .parent_id = 15,
      .types = 0,
      .preview = NULL,
      .file_format = "afs/unknown",
      .metadata = NULL,
  };
  gvector file_data = {
      .buffer = _________ROAEshell_db_simpledb_siard,
      .size = 17373,
      .item_size = 1,
      .element_free = NULL,
  };

  ivm_file_format_siard_render(NULL, &file, &file_data);
  return 0;
}
