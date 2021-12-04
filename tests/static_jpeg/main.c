/*****************************************************************************
**
**  Decode and render a JPEG stored in a C array
**
**  Creation date:  2020/05/15
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2020 Piql AS. All rights reserved.
**
**  This file is part of the ivm-app
**
*****************************************************************************/

//  DEFINES
//
#define BOXINGLIB_CALLBACK
#define LOGGING_ENABLED

//  PROJECT INCLUDES
//
#include "fish_n_chips-x12.h"
#include "ivm_io.h"

#include "ivm_formats/ivm_formats.h"
#include <string.h>
#include <stdio.h>
//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 * 
 *  Unboxing sample application.
 *
 */

int main(int argc, char *argv[])
{
  gvector* data = gvector_create(1, jpeg_fish_n_chips_x12_jpeg_len);
  memcpy(data->buffer, jpeg_fish_n_chips_x12_jpeg, data->size);

  ivm_file_format* jpeg = ivm_file_format_find("afs/jpeg");
  if (jpeg == NULL)
  {
      printf("ERROR: Failed to create JPEG file format\n");
      return 1;
  }
  int ok = jpeg->render(jpeg, NULL, data);
  
  return 0;
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
