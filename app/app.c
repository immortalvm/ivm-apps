#include "afs/afsutil.h"
#include "afs/tocfile.h"
#include "ivm_io/ivm_io.h"
#include "ivm_formats/ivm_formats.h"
#include <stdio.h>

#include "inputdevice.h"

static ivm_file_format* get_ivm_format(afs_toc_file* file);

int read_and_exec_app(afs_control_data* control_data, DBOOL is_raw)
{
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
        printf("unbox file result=%d\n", result);
        
        // Render file
        if (result == BOXING_UNBOXER_OK)
        {
          printf("rendering %s\n", toc_file->name);
          ivm_format->render(ivm_format, toc_file, data);
        }

        gvector_free(data);
      }
    }
  }
  afs_toc_data_free(toc_data);
    
  return 0;
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
