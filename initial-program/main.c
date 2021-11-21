#include "boxing/image8.h"
#include "afs/afsutil.h"
#include "afs/tocfile.h"
#include <stdio.h>

#include "settings.h"
#include "inputdevice.h"

// The compiled version of the *initial-program* should be stored on the stoarage medium
// as a human readable binary stream, typically encoded using some form of hex-encoding.
//
// Experimental:
// The *app* can either be included as part of the initial program (IVM_INCLUDE_APP defined)
// or be dynamically loaded and executed from the storage medium. If set up for dynamic
// loading, the app will be smaller and hence the human readable decoded version of the
// app will be smaller, requireing less space on the storage medium.
#if IVM_INCLUDE_APP
#include "app.h"
#else
#include "afs/tocfile.h"
#endif

#if IVM_RAW_TESTDATA  
  DBOOL is_raw = DTRUE;
#else
  DBOOL is_raw = DFALSE;
#endif  
  

int main(int argc, char** argv)
{
  printf("starting ivm-app\n");

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

#if IVM_INCLUDE_APP
  result = read_and_exec_app(control_data, is_raw);
#else
  // Read application from film and execute
  // Todo: Experimental code only
  /*  
  if (control_data->technical_metadata->afs_applications)
  {
    toc_data_file* app = control_data->technical_metadata->afs_applications[0];
    gvector* data = gvector_create(1, app->size);
    afs_util_unbox_file_parameters file_params;
    file_params.save_file = afs_util_gvector_save_file_callback;
    file_params.control_data = control_data;
    file_params.get_image = get_image;
    file_params.is_raw = is_raw;
        
    // Unbox file
    int result = afs_util_unbox_file(toc_file, &file_params, data);
    printf("unbox app result=%d\n", result);
        
    // Todo: Exec app
    // Free
    gvector_free(data);
  }
  else
  {
    printf("No application found in control frame, exiting\n");
  }
  */
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
