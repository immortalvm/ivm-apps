#include "ivm_io/ivm_io.h"
#include "boxing/image8.h"
#include "afs/afsutil.h"
#include "afs/tocfile.h"
#include "ivm_formats/ivm_formats.h"
#include <stdio.h>

#define LOGGING_ENABLED 1

static int get_image(boxing_image8** image, unsigned frame);
static int save_file(void* user, int position, unsigned char* data, unsigned long size);
static ivm_file_format* get_ivm_format(afs_toc_file* file);
static int read_and_exec_app(afs_control_data* control_data, DBOOL is_raw);

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

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
  result = read_and_exec_app(control_data, is_raw);
  afs_control_data_free(control_data);
  return result;
}

int read_and_exec_app(afs_control_data* control_data, DBOOL is_raw)
{
  // Check if control data has an iVM block

  
  // Code below here should be on film
  
  // Decode TOC
  afs_util_unbox_toc_parameters toc_params;
  toc_params.control_data = control_data;
  toc_params.get_image = get_image;
  toc_params.is_raw = is_raw;

  afs_toc_file * toc_file = afs_toc_files_get_toc(control_data->technical_metadata->afs_tocs, 0);
  char * toc_file_string = afs_toc_file_save_string(toc_file, DFALSE, DFALSE);
  printf("Table of content location:\n");
  printf("%s\n", toc_file_string);
  boxing_string_free(toc_file_string);
  
  afs_toc_data* toc_data;
  int result = afs_util_unbox_toc(&toc_data, &toc_params);
  if (result != 0)
  {
    printf("toc extraction failed\n");
    return 1;
  }
  char * toc_data_string = afs_toc_data_save_string(toc_data, DFALSE);
  printf("Table of content:\n");
  printf("%s\n", toc_data_string);
  boxing_string_free(toc_data_string);

  // Decode all supported files
  unsigned int reel_count = afs_toc_data_reel_count(toc_data);
  printf("Found %u reels\n", reel_count);
  if (reel_count)
  {
    afs_toc_data_reel* reel = afs_toc_data_get_reel_by_index(toc_data, 0);

    unsigned int file_count = afs_toc_data_reel_file_count(reel);

    printf("Found %u files\n", file_count);
    
    for (unsigned int f = 0; f < file_count; f++)
    {
      afs_toc_file* toc_file = afs_toc_data_reel_get_file_by_index(reel, f);

      ivm_file_format* ivm_format = get_ivm_format(toc_file);

      printf("%s: format=%s size=%ld\n", toc_file->name, toc_file->file_format, toc_file->size);
      
      if (ivm_format != NULL)
      {
        gvector* data = gvector_create(1, toc_file->size);
        afs_util_unbox_file_parameters file_params;
        file_params.save_file = afs_util_gvector_save_file_callback;
        file_params.control_data = control_data;
        file_params.get_image = get_image;
        file_params.is_raw = is_raw;
        
        // Unbox file
        int result = afs_util_unbox_file(toc_file, &file_params, data);
        
        // Render file
        if (result == BOXING_UNBOXER_OK)
        {
          ivm_format->render(ivm_format, toc_file, data);
        }

        gvector_free(data);
      }
    }
  }

  return 0;
}

static DBOOL get_image(boxing_image8** image, unsigned frame)
{
    printf("reading image %u\n",frame);

    long width;
    long height;
    ivm64_read_frame(&width, &height, frame);
    if (width == 0 || height == 0)
    {
        printf("Illegal image size: %ldx%ld\n", width, height);
        return DFALSE;
    }
    
    *image = boxing_image8_create(width, height);
    if (*image == NULL)
    {
        printf("Failed to allocate image with size: %ldx%ld\n", width, height);
        return DFALSE;
    }

    boxing_image8_pixel * data = (*image)->data;
    for (long y = 0; y < height; y++)
    {
        for (long x = 0; x < width; x++)
        {
            uint8_t pixel = ivm64_read_pixel(x, y);
            
            *data = (boxing_image8_pixel)(pixel);
            data++;
        }
    }

    printf("reading image %u complete\n",frame);
    return DTRUE;
}


static ivm_file_format* get_ivm_format(afs_toc_file* file)
{
  unsigned format_count = ivm_file_format_count();
  for (unsigned i = 0; i < format_count; i++)
  {
    ivm_file_format* format = ivm_file_format_get(i);

    if (boxing_string_equal(file->file_format, format->file_format))
    {
      return format;
    }
  }

  return DFALSE;
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
