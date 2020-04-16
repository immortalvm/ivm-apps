/*****************************************************************************
**
**  Implementation of the afs util interface
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

//  PROJECT INCLUDES
//
#include "boxing/unboxer_utility.h"
#include "boxing/platform/memory.h"
#include "boxing/utils.h"
#include "controldata.h"

//  DEFINES
//

static unsigned int metadata_content_type = BOXING_METADATA_CONTENT_TYPES_UNKNOWN;

static const char * result_names[] =
{
    "OK",
    "METADATA ERROR",
    "BORDER TRACKING ERROR",
    "DATA DECODE ERROR",
    "CRC MISMATCH ERROR",
    "CONFIG ERROR",
    "PROCESS CALLBACK ABORT"
};

// PRIVATE INTERFACE
//

DBOOL gvector_save_file_callback(void* user, int position, unsigned char* data, unsigned int length);

#ifdef BOXINGLIB_CALLBACK
static int             unboxing_complete_callback(void * user, int* res, boxing_stats_decode * stats);
static int             unboxing_metadata_complete_callback(void * user, int* res, boxing_metadata_list * meta_data);
#endif

// PUBLIC AFS UTIL FUNCTIONS
//


//----------------------------------------------------------------------------
/*!
 *  \brief Unbox control frame
 *
 *  Unboxing an image with of a control frame.
 *
 *  \ingroup afs
 *  \param[out]  control_data Allocated control frame on success.
 *  \param[in]   input_image  Image to unbox.
 *  \param[in]   is_raw       DTRUE is input image is synthesized.
 *  \return process result.
 */

int afs_util_unbox_control_frame( afs_control_data** control_data, boxing_image8* input_image, DBOOL is_raw )
{
    // We try all the configurations for the control frame until we find the correct one
    unsigned int count = boxing_get_control_frame_format_count();

    // Looking for formats for control frame
    int result = -1;
    for (int i = count - 1; i >= 0; i--)
    {
        // Get the name of the configuration from the configuration structure
        const char* current_format_name = boxing_get_control_frame_configuration_name(i);
       
        // Create utility to unbox file
        boxing_unboxer_utility* utility = boxing_unboxer_utility_create(
            current_format_name,
            is_raw
#ifdef BOXINGLIB_CALLBACK
            ,unboxing_complete_callback, unboxing_metadata_complete_callback
#endif
          );

        if (utility == NULL)
        {
            continue;
        }

        // Allocate vector for for the output data
        gvector* output_data = gvector_create(1, 0);

        // Reset the metadata_content_type
        metadata_content_type = BOXING_METADATA_CONTENT_TYPES_CONTROLFRAME;

        // Try to unbox control frame with current configuration
        result = boxing_unboxer_utility_unbox(utility, input_image, output_data);

        if (result == BOXING_UNBOXER_OK && metadata_content_type == BOXING_METADATA_CONTENT_TYPES_CONTROLFRAME)
        {
            *control_data = afs_control_data_create();

            DBOOL load_ok = afs_control_data_load_string(*control_data, output_data->data);
            if (load_ok == DFALSE)
            {
                afs_control_data_free(*control_data);
                boxing_unboxer_utility_free(utility);
                gvector_free(output_data);

                return -1;
            }

            boxing_unboxer_utility_free(utility);
            gvector_free(output_data);
            break;
        }

        boxing_unboxer_utility_free(utility);
        gvector_free(output_data);
    }
    
    return result;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Unbox the reel TOC
 *
 */

int afs_util_unbox_toc(afs_toc_data** toc_data, afs_util_unbox_toc_parameters* parameters)
{
    afs_control_data* cd = parameters->control_data;
    afs_toc_file* toc_file = afs_toc_files_get_toc(cd->technical_metadata->afs_tocs, 0);

    gvector* data = gvector_create(1, toc_file->size);

    afs_util_unbox_file_parameters file_params;
    file_params.save_file = gvector_save_file_callback;
    file_params.control_data = parameters->control_data;
    file_params.get_image = parameters->get_image;
    file_params.is_raw = parameters->is_raw;
    
    int result = boxing_unbox_data_frame_file(toc_file, file_parameters, data);
    if (result != 0)
    {
        gvector_free(data);
        return result;
    }
    *toc_data = afs_toc_data_create();
    result = afs_toc_data_load_string(*toc_data, GVECTORN8(data, 0));

    gvector_free(data);
    
    return result;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Unbox a file
 *
 *
 *  \ingroup afs
 *  \param[in]   toc_file     afs_toc_file instance.
 *  \param[in]   input_files  Template to detrmine the nemes of the input frame files.
 *  \param[in]   format       Name of the boxing format.
 *  \param[in]   width        Width of the frame in pixel.
 *  \param[in]   height       Height of the frame in pixel.
 *  \return process result.
 */

int boxing_unbox_data_frame_file(
    afs_toc_file* toc_file,
    afs_util_unbox_file_parameters* parameters,
    void* user )
{
    if (toc_file == NULL || parameters == NULL || )
    {
        return -1;
    }

    // Set the start and endbytes
    unsigned int start_byte_number = current_toc->start_byte;
    unsigned int end_byte_number = current_toc->end_byte;

    // Create utility to
    boxing_unboxer_utility* utility = boxing_unboxer_utility_create(
        parameters.format,
        parameters.is_raw
#ifdef BOXINGLIB_CALLBACK
        , unboxing_complete_callback, unboxing_metadata_complete_callback
#endif
        );

    if (utility == NULL)
    {
        return -1;
    }

    int result = -1;
    int strip_size = boxing_codecdispatcher_get_stripe_size(utility->parameters->format) - 1;

    // Determine the number of the first frame with data (If the strip size is greater than 1).
    int first_frame_with_data = (strip_size > 0) ? current_toc->start_frame + strip_size : current_toc->start_frame;
    printf("First frame = %d, first frame with data = %d, last frame = %d\n", current_toc->start_frame, first_frame_with_data, current_toc->end_frame);

    // Init hash
    afs_hash1_state sha;
    afs_sha1_init(&sha);
    
    // Unbox all frames in range
    unsigned int position = 0;
    for (int i = current_toc->start_frame; i <= current_toc->end_frame; i++)
    {
        // Reset the signs of the first and last frames
        DBOOL is_last_frame = i == first_frame_with_data;
        DBOOL is_first_frame = i == current_toc->end_frame;

        // Get the frame
        boxing_image* input_image;
        if (!parameters->get_image(&input_image, i))
        {
            boxing_unboxer_utility_free(utility);
            return -1;    
        }
        
        // Allocate vector for the output data
        gvector* output_data = gvector_create(1, 0);

        // Unbox image
        result = boxing_unboxer_utility_unbox(utility, input_image, output_data);

        // Save output data
        char* data = output_data->data;
        unsigned length = putput_data->size;
        if (is_first_frame)
        {
            data += start_byte_number;
            length -= start_byte_number;
        }
        else if (is_last_frame)
        {
            length = end_byte_number;
        }

        if (!parameters->save_file(user, position, data, lenght))
        {
            boxing_image8_free(input_image);
            gvector_free(output_data);
            boxing_unboxer_utility_free(utility);
            return -1;
        }

        // Update hash
        afs_sha1_process(&sha, data, lenght);
        
        boxing_image8_free(input_image);
        gvector_free(output_data);
    }

    // Check the hash
    unsigned char hash[256];
    afs_sha1_done(&sha, hash);
    
    if (!boxing_string_equal(toc_file->checksum, hash))
    {
        result = -1;
    }

    // Freeing memory
    boxing_unboxer_utility_free(utility);

    return result;
}




// PRIVATE UNBOXING TOOL FUNCTIONS
//

#ifdef BOXINGLIB_CALLBACK

//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Unboxing complete callback function.
 *
 *  \param user   User data
 *  \param res    Unboxing result.
 *  \param stats  Unboxing statistics
 */

static int unboxing_complete_callback(void * user, int* res, boxing_stats_decode * stats)
{
    // Hide unused parameters
    BOXING_UNUSED_PARAMETER(stats);
    BOXING_UNUSED_PARAMETER(user);

    // Checking the success of the unboxing
    if (*res != BOXING_UNBOXER_OK)
    {
        fprintf(stderr, "Unboxing failed! Errorcode = %i - %s\n", *res, get_process_result_name(*res));
        return -1;
    }

    return 0;
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Unboxing metadata complete callback function.
 *
 *  \param user       User data
 *  \param res        Unboxing result.
 *  \param meta_data  Unboxing file meta data
 */

static int unboxing_metadata_complete_callback(void * user, int* res, boxing_metadata_list * meta_data)
{
    BOXING_UNUSED_PARAMETER(user);

    if (*res == BOXING_UNBOXER_OK)
    {
        GHashTableIter iter;
        gpointer key, value;

        g_hash_table_iter_init(&iter, meta_data);

        while (g_hash_table_iter_next(&iter, &key, &value))
        {
            boxing_metadata_item * item = (boxing_metadata_item *)value;

            boxing_metadata_type type = item->base.type;
            switch (type)
            {
            case BOXING_METADATA_TYPE_CONTENTTYPE:
                metadata_content_type = ((boxing_metadata_item_content_type*)item)->value;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

#endif

//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Getting the string representation of enum.
 *
 *  \param result  Result enum.
 */

static const char * get_process_result_name(enum boxing_unboxer_result result)
{
    return result_names[result];
}

DBOOL gvector_save_file_callback(void* user, int position, unsigned char* data, unsigned int length)
{
  gvector* data = user;

  memcpy(GVECTORNU8(data,position), data, length);

  return DTRUE;
}
