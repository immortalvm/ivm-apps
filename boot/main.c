#include "boxing/image8.h"
#include "afs/afsutil.h"
#include "afs/tocfile.h"
#include <stdio.h>

#include "settings.h"
#include "inputdevice.h"

#if IVM_INCLUDE_APP
#include "app.h"
#endif

//#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char** argv)
{
  // Decompress rest of application
  printf("starting ivm-app\n");

  // Code below here should be compressed...
  
  // Assume for now all images are raw
  DBOOL is_raw = DTRUE;
  
  // Read control frame from input device
  boxing_image8* cf;
  if (get_image(&cf, 1) != DTRUE)
  {
    return 1;
  }

  // Decode control frame
  afs_util_unbox_cf_parameters cf_params;
  cf_params.formats = afs_util_control_frame_formats();
  cf_params.format_count = afs_util_control_frame_format_count();
  cf_params.is_raw = is_raw;

  afs_control_data* control_data;
  int result = afs_util_unbox_control_frame(&control_data, cf, &cf_params);
  if (result != 0)
  {
      printf("control frame extraction failed\n");
      return 1;
  }

  boxing_image8_free(cf);
  cf = NULL;

  // Read application from film and decompress
#if IVM_INCLUDE_APP
  result = read_and_exec_app(control_data, is_raw);
#endif
  
  afs_control_data_free(control_data);
  return result;
}


#if defined (LOGGING_ENABLED)
#include <stdarg.h>

void boxing_log(int log_level, const char * string)
{
    printf("%d : %s\n", log_level, string);
}

void boxing_log_args(int log_level, const char * format, ...)
{
    va_list args;
    va_start(args, format);

    printf("%d : ", log_level);
    vprintf(format, args);
    printf("\n");

    va_end(args);
}
#else
void boxing_log(int log_level, const char * string) {}
void boxing_log_args(int log_level, const char * format, ...) {}
#endif // LOGGING_ENABLED

void(*boxing_log_custom)(int log_level, const char * string) = NULL;
void(*boxing_log_args_custom)(int log_level, const char * format, va_list args) = NULL;
