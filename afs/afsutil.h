#ifndef AFS_UTIL_H
#define AFS_UTIL_H

/*****************************************************************************
**
**  Definition of the afs util interface
**
**  Creation date:  2020/04/15
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2020 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "boxing/unboxer_utility.h"
#include "afs/controldata.h"
#include "afs/tocdata_c.h"

// TYPEDEFS
// 
typedef DBOOL (*afs_get_image_callback)(boxing_image8** image, unsigned int frame);
typedef DBOOL (*afs_save_file_callback)(void* user, int position, unsigned char* data, unsigned long length);

typedef struct afs_util_unbox_cf_parameters_s
{
    boxing_config**        formats;
    unsigned               format_count;
    DBOOL                  is_raw;
} afs_util_unbox_cf_parameters;
  
typedef struct afs_util_unbox_toc_parameters_s
{
    afs_control_data*      control_data;
    afs_get_image_callback get_image;
    DBOOL                  is_raw;
} afs_util_unbox_toc_parameters;
  
typedef struct afs_util_unbox_file_parameters_s
{
    afs_control_data*      control_data;
    afs_get_image_callback get_image;
    afs_save_file_callback save_file;
    DBOOL                  is_raw;
} afs_util_unbox_file_parameters;


int afs_util_gvector_save_file_callback(void* user, int position, unsigned char* data, unsigned long length);
  
unsigned        afs_util_control_frame_format_count();
boxing_config*  afs_util_control_frame_format(unsigned index);
boxing_config** afs_util_control_frame_formats();

int afs_util_unbox_control_frame(afs_control_data** control_data, boxing_image8* input_image, afs_util_unbox_cf_parameters* parameters);
int afs_util_unbox_toc(afs_toc_data** toc, afs_util_unbox_toc_parameters* parameters);
int afs_util_unbox_file(afs_toc_file* toc_file, afs_util_unbox_file_parameters* parameters, void* user);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // UNBOXING_TOOL_H
