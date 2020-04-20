#include "ivm_io/ivm_io.h"
#include "afs/afsutil.h"
#include "boxing/image8.h"
#include "tocfile.h"
#include "ivm_formats/ivm_formats.h"

static int get_image(boxing_image8** image, unsigned frame);
static int save_file(void* user, int position, unsigned char* data, unsigned long size);
static ivm_file_format* get_ivm_format(afs_toc_file* file);

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char** argv)
{
  // Decompress rest of application


  // Code below here should be compressed...
  
  // Assume for now all images are raw
  DBOOL is_raw = DTRUE;
  
  // Read control frame from input device
  boxing_image8* cf;
  if (get_image(&cf, 0) != DTRUE)
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
      return 1;
  }

  boxing_image8_free(cf);
  cf = NULL;

  // Read application from film and decompress
  
  
  // Code below here should be on film
  
  // Decode TOC
  afs_util_unbox_toc_parameters toc_params;
  toc_params.control_data = control_data;
  toc_params.get_image = get_image;
  toc_params.is_raw = is_raw;

  afs_toc_file * toc_file = afs_toc_files_get_toc(control_data->technical_metadata->afs_tocs, 0);

  afs_toc_data* toc_data;
  result = afs_util_unbox_toc(&toc_data, &toc_params);
  if (result != 0)
  {
      return 1;
  }

  // Decode all supported files
  unsigned int reel_count = afs_toc_data_reel_count(toc_data);
  if (reel_count)
  {
    afs_toc_data_reel* reel = afs_toc_data_get_reel_by_index(toc_data, 0);

    unsigned int file_count = afs_toc_data_reel_file_count(reel);

    for (unsigned int f = 0; f < file_count; f++)
    {
      afs_toc_file* toc_file = afs_toc_data_reel_get_file_by_index(reel, f);

      ivm_file_format* ivm_format = get_ivm_format(toc_file);
      
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
}


static DBOOL get_image(boxing_image8** image, unsigned frame)
{
    long width;
    long height;
    ivm64_read_frame(frame, &width, &height);

    if (width == 0 || height == 0)
    {
        return DFALSE;
    }

    *image = boxing_image8_create(width, height);
    if (*image == NULL)
    {
        return DFALSE;
    }

    boxing_image8_pixel * data = (*image)->data;
    for (long y = 0; y < height; y++)
    {
        for (long x = 0; x < width; x++)
        {
            long r,g,b;
            ivm64_get_pixel(x, y, r, g, b);

            *data = (boxing_image8_pixel)(r & 0xff);
            data++;
            *data = (boxing_image8_pixel)(g & 0xff);
            data++;
            *data = (boxing_image8_pixel)(b & 0xff);
            data++;
        }
    }

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
