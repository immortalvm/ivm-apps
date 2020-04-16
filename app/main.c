#include "ivm_io/ivm_io.h"
#include "afs/afsutil.h"
#include "boxing/image8.h"
#include "tocfile.h"

typedef struct
{
    unsigned char* data;
    unsigned long size;
} ivm_file;

// Current file being decoded
static ivm_file* current_file;

static int get_image(boxing_image8** image, unsigned frame);
static int save_file(void* user, int position, unsigned char* data, unsigned long size);
static void init_file(ivm_file* file, afs_toc_file* toc_file);


// Vector of supported file formats


int main(int argc, char** argv)
{
  // Decompress rest of application

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
  cf_params.formats = &
  cf_params.format_count = 1;
  cf_params.is_raw = is_raw;

  afs_control_data* control_data;
  int result = afs_util_unbox_control_frame(&control_data, cf, is_raw);
  if (result != 0)
  {
      return 1;
  }
  
  // Decode TOC
  afs_util_unbox_toc_parameters toc_params;
  toc_params.control_data = control_data;
  toc_params.get_image = get_image;
  toc_params.is_raw = is_raw;

  afs_toc_file * toc_file = afs_toc_files_get_toc(control_data->technical_metadata->afs_tocs, 0);
  init_file(current_file, toc_file);

  afs_toc_data* tocdata;
  result = afs_util_unbox_toc(&tocdata, &toc_params);
  if (result != 0)
  {
      return 1;
  }

  // Decode all supported files

  
  
  // Read image from input device
  
  // Unbox image

  // Decode image

  // Render image to output device
  
}

static void init_file(ivm_file* file, afs_toc_file* toc_file)
{
  file->size = afs_toc_file_size(toc_file);
  file->data = malloc(file->size);
}


static int get_image(boxing_image8** image, unsigned frame)
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
