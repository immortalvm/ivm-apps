/*****************************************************************************
**
**  Decode and render a TIFF stored in a C array
**
**  Creation date:  2020/04/07
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
#include "fish_n_chips-x100.h"
#include "ivm_io.h"

#include <tiffio.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef struct {
  unsigned int    size;
  unsigned char * data;
  unsigned int    pos;
} tiff_file;


tsize_t tiff_read(thandle_t st,tdata_t buffer,tsize_t size)
{
  tiff_file* file = (tiff_file *)st;

  tsize_t n;
  if (((tsize_t) file->pos + size) <= file->size) {
    n = size;
  }
  else {
    n = file->size - file->pos;
  }
  
  memcpy(buffer, file->data + file->pos, n);
  file->pos += n;

  return n;
}

tsize_t tiff_write(thandle_t st,tdata_t buffer,tsize_t size)
{
  return 0;
}

int tiff_close(thandle_t st)
{
  return 0;
};

toff_t tiff_seek(thandle_t st,toff_t pos, int whence)
{
  tiff_file* file = (tiff_file *)st;
  switch (whence) {
  case SEEK_SET: {
    if ((tsize_t) pos > file->size) {
      //file->data = (unsigned char *) realloc(memtif->data, memtif->size + memtif->incsiz + off);
      //file->size = file->size + file->incsiz + off;
      return -1;
    }
    file->pos = pos;
    break;
  }
  case SEEK_CUR: {
    if ((tsize_t)(file->pos + pos) > file->size) {
      //file->data = (unsigned char *) realloc(file->data, file->fptr + file->incsiz + off);
      //file->size = file->pos + file->incsiz + off;
      return -1;
    }
    file->pos += pos;
    break;
  }
  case SEEK_END: {
    if ((tsize_t) (file->size + pos) > file->size) {
      //memtif->data = (unsigned char *) realloc(memtif->data, memtif->size + memtif->incsiz + off);
      //memtif->size = memtif->size + memtif->incsiz + off;
      return -1;
    }
    file->pos = file->size - pos;
    break;
  }
  }
  //if (file->pos > file->size) memtif->size = memtif->pos;
  return file->pos;  
  
};

toff_t tiff_size(thandle_t st)
{
  tiff_file* file = (tiff_file *) st;
  return file->size;
};

int tiff_map(thandle_t st, tdata_t* data, toff_t* off)
{
    return 0;
};

void tiff_unmap(thandle_t st, tdata_t data, toff_t off)
{
    return;
};


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 * 
 *  TIFF test application.
 *
 */

int main(int argc, char *argv[])
{
  printf( "LIBTIFF version: %d\n", TIFFLIB_VERSION );

  int dimension = 12;
  
  // Sample file
  tiff_file file;
  if (dimension == 12) {
    file.size = tiff_fish_n_chips_x12_tiff_len;
    file.data = tiff_fish_n_chips_x12_tiff;
  } else if (dimension == 100) {
    file.size = tiff_fish_n_chips_x100_tiff_len;
    file.data = tiff_fish_n_chips_x100_tiff;    
  }
  file.pos = 0;
  
  // Open form memory
  thandle_t userdata = &file;
  TIFF* tif = TIFFClientOpen(
    "Memory", "r", userdata,
    tiff_read, tiff_write, tiff_seek, tiff_close, tiff_size,
    tiff_map, tiff_unmap);
  
  if (tif) {
    uint32 w, h;
    size_t npixels;
    uint32* raster;
        
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);

    printf( "Dimensions: %dx%d\n", w, h );

    npixels = w * h;
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster != NULL) {
      if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {

        ivm64_new_frame( w, h, 0 );

        uint32* p = raster;
        for (uint32 y = 0; y < h; y++ ) {
          for (uint32 x = 0; x < w; x++ ) {
            ivm64_set_pixel(x, y, TIFFGetR(*p), TIFFGetG(*p), TIFFGetB(*p));
            p++;
          }
        }
      }
      _TIFFfree(raster);
    }
    TIFFClose(tif);
  }
  
  return 0;
}

