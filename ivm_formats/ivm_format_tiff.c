#include "ivm_format_tiff.h"

#include <tiffio.h>
#include "ivm_io/ivm_io.h"
#include "ivm_formats/ivm_formats.h"
#include "afs/tocfile.h"

typedef struct {
  unsigned int    size;
  unsigned char * data;
  unsigned int    pos;
} tiff_file;


static tsize_t tiff_read(thandle_t st,tdata_t buffer,tsize_t size);
static tsize_t tiff_write(thandle_t st,tdata_t buffer,tsize_t size);
static int tiff_close(thandle_t st);
static toff_t tiff_seek(thandle_t st,toff_t pos, int whence);
static toff_t tiff_size(thandle_t st);
static int tiff_map(thandle_t st, tdata_t* data, toff_t* off);
static void tiff_unmap(thandle_t st, tdata_t data, toff_t off);


int ivm_file_format_tiff_render(ivm_file_format* format, afs_toc_file* toc_file, gvector* data)
{
  // Sample file
  tiff_file file;
  file.size = data->size;
  file.data = data->buffer;
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

    printf( "TIFF dimensions: %dx%d\n", w, h );

    npixels = w * h;
    printf( "TIFF size: %lu\n", npixels * sizeof(uint32) );
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster != NULL) {
      if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
        printf( "TIFF read image successfully\n" );

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


static tsize_t tiff_read(thandle_t st,tdata_t buffer,tsize_t size)
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

static tsize_t tiff_write(thandle_t st,tdata_t buffer,tsize_t size)
{
  return 0;
}

static int tiff_close(thandle_t st)
{
  return 0;
}

static toff_t tiff_seek(thandle_t st,toff_t pos, int whence)
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
  
}

static toff_t tiff_size(thandle_t st)
{
  tiff_file* file = (tiff_file *) st;
  return file->size;
}

static int tiff_map(thandle_t st, tdata_t* data, toff_t* off)
{
    return 0;
}

static void tiff_unmap(thandle_t st, tdata_t data, toff_t off)
{
    return;
}

