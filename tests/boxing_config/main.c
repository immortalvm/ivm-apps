// Compile: ivm64-gcc main.c -I ../../file-format-decoders/include/
/*****************************************************************************
**
**  Test the boxing_config
**
**  Creation date:  2020/04/24
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2020 Piql AS. All rights reserved.
**
**  This file is part of the ivm-app
**
*****************************************************************************/

#include "boxing/config.h"
#include "config_source_4k_controlframe_v7.h"
#include "stdio.h"
#include "stdarg.h"

//  DEFINES
//
#define BOXINGLIB_CALLBACK
#define LOGGING_ENABLED

//  PROJECT INCLUDES
//

//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 * 
 *  Unboxing sample application.
 *
 */

int main(int argc, char *argv[])
{
  boxing_config* bc = boxing_config_create_from_structure(&config_source_v7);
  
  return 0;
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

