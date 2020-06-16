#include "ivm_io.h"

#ifdef __ivm64__
#include <assert.h>
static_assert(sizeof(long) == 8, "8 bytes in a long");
#else
#include <stdio.h>
#include <png.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Error codes
#define OPTION_PARSE_ERROR 1
#define NOT_READABLE 2
#define NOT_IMPLEMENTED 3
#define UNDEFINED_INSTRUCTION 4
#define OUT_OF_MEMORY 5
#define STRING_TOO_LONG 6
#define NOT_WRITEABLE 7
#define PNG_TROUBLE 8

#define MAX_FILENAME 260

typedef struct {
  void* array;
  size_t size;
  size_t used;
} Space;

static char* inpDir = NULL;
static char* outDir = NULL;
static unsigned readframeindex;
static struct dirent** inpFiles;
static int numInpFiles;
static int nextInpFile = 0;
static Space currentInImage;
static uint16_t currentInWidth;
static uint16_t currentInHeight = 0;
static size_t currentInRowbytes = 0;
static Space currentInRowpointers;

static void spaceInit(Space* s);
static void spaceReset(Space* s, size_t needed);
static int acceptPng(const struct dirent* entry);
static void ioInitIn();
static void ioReadFrame();
static uint8_t ioReadPixel(uint16_t x, uint16_t y);

#endif

void ivm64_new_frame(long width, long height, long rate)
{
#ifdef __ivm64__
  asm volatile("new_frame* [(load8 %[w]) (load8 %[h]) (load8 %[r])]"
               : : [w] "m" (width), [h] "m" (height), [r] "m" (rate));
#else
  printf ("new_frame* [%ld %ld %ld]\n", width, height, rate);
#endif
}

void ivm64_set_pixel(long x, long y, long r, long g, long b) 
{
#ifdef __ivm64__
  asm volatile("set_pixel* [(load8 %[x]) (load8 %[y]) (load8 %[r]) (load8 %[g]) (load8 %[b])]"
               : : [x] "m" (x), [y] "m" (y), [r] "m" (r), [g] "m" (g), [b] "m" (b));
#else
  printf ("set_pixel* [%ld %ld %ld %ld %ld])\n", x, y, r, g, b);
#endif
}

void ivm64_read_frame(long* widthp, long* heightp, long frameindex) 
{
  long width = 0, height = 0;
  /* Could we avoid this intermediate storage,
     and instead write directly to the args in the asm below? */

#ifdef __ivm64__
  printf ("read_frame index=%lu\n",frameindex);

  /* Pushes 16 bytes on the stack (first width, then height): */
  asm volatile("read_frame! (load8 %[n])" : : [n] "m" (frameindex));
  /* Pops 8 bytes form the stack: */
  asm volatile("store8! (+ 16 %[h])" : [h] "=m" (height) : );
  /* Idem: */
  asm volatile("store8! (+  8 %[w])" : [w] "=m" (width) : );
#else
  printf ("read_frame index=%lu\n",frameindex);

  static int init;
  if (init == 0) {
    ioInitIn();
    init = 1;
  }
  
  ioReadFrame(frameindex);
  width = currentInWidth;
  height = currentInHeight;
  
  printf ("<store8-ed values: width=%ld, height=%ld>\n", width, height);
#endif

  *widthp = width;
  *heightp = height;
}

long ivm64_read_pixel(long x, long y)
{
  long v = 0;

#ifdef __ivm64__
  asm volatile("read_pixel* [(load8 %[x]) (load8 %[y])]"
               : : [x] "m" (x), [y] "m" (y) ); /* Pushes 8 bytes on the stack */
  asm volatile("store8! (+ 8 %[v])"
               : [v] "=m" (v) : ); /* Pops 8 bytes */
#else
  //printf ("read_pixel* [%ld %ld])\n", x, y);
  //printf ("<result value on stack: %ld>\n", v);

  v = ioReadPixel(x, y);
#endif

  return v;
}

#ifndef __ivm64__

// Emulated functions
static void spaceInit(Space* s) {
  s->array = NULL;
  s->size = s->used = 0;
}

static void spaceReset(Space* s, size_t needed) {
  s->used = needed;
  if (s->size >= needed) return;
  free(s->array);
  s->array = malloc(needed);
  if (!s->array) exit(OUT_OF_MEMORY);
  s->size = needed;
}


static int acceptPng(const struct dirent* entry) {
  char* ext;
  return (entry->d_type == DT_REG)
    && (ext = strrchr(entry->d_name, '.'))
    && strcmp(ext, ".png") == 0;
}

static void ioInitIn() {
  // Get input folder from env
  inpDir = getenv( "IVM_EMULATOR_INPUT" );
  if (inpDir == NULL ) {
    printf("Error: No input files are defined using IVM_EMULATOR_INPUT\n");
  }

  numInpFiles = inpDir ? scandir(inpDir, &inpFiles, acceptPng, alphasort) : 0;
  if (numInpFiles < 0) {
    perror("scandir");
    exit(NOT_READABLE);
  }
  spaceInit(&currentInImage);
  spaceInit(&currentInRowpointers);
}


// Sets currentInWidth and ..Height instead of returning values.
static void ioReadFrame(long frameindex) {
  nextInpFile = frameindex;
  if (nextInpFile >= numInpFiles) {
    currentInWidth = 0;
    currentInHeight = 0;
    return;
  }
  static char filename[MAX_FILENAME];
  struct dirent* f = inpFiles[nextInpFile++];
  sprintf(filename, "%s/%s", inpDir, f->d_name);
  //free(f);

  printf("Opening file %s\n", filename);
  
  FILE *fileptr;
  png_structp png;
  png_infop info;
  if (!(fileptr = fopen(filename, "rb"))
  || !(png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL))
  || !(info = png_create_info_struct(png))
  || setjmp(png_jmpbuf(png))) {
    perror("png read error");
    exit(NOT_READABLE);
  }
  png_init_io(png, fileptr);
  png_read_info(png, info);
  currentInWidth = png_get_image_width(png, info);
  currentInHeight = png_get_image_height(png, info);

  if (png_get_bit_depth(png, info) == 16) {
    png_set_strip_16(png);
  }
  png_byte color_type = png_get_color_type(png, info);
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
    png_set_rgb_to_gray(png, 1, -1.0, -1.0); // Default weights
  }
  if (color_type & PNG_COLOR_MASK_ALPHA) {
    png_set_strip_alpha(png);
  }
  png_read_update_info(png, info);
  size_t rowbytes = png_get_rowbytes(png, info);
  size_t needed = rowbytes * currentInHeight;

  // We have attempted to optimize for the expected case
  // when all the inp frames have the same format.

  if (needed > currentInImage.size) {
    spaceReset(&currentInImage, needed);
    currentInRowbytes = 0;
  }
  if (currentInHeight > currentInRowpointers.size) {
    spaceReset(&currentInRowpointers, currentInHeight * sizeof(void*));
    currentInRowbytes = 0;
  }
  if (rowbytes != currentInRowbytes) {
    void** rp = currentInRowpointers.array;
    for (int y = 0; y < currentInHeight; y++) {
      rp[y] = currentInImage.array + rowbytes * y;
    }
    currentInRowbytes = rowbytes;
  }

  png_read_image(png, currentInRowpointers.array);
  fclose(fileptr);
  png_destroy_read_struct(&png, &info, NULL);
}

static uint8_t ioReadPixel(uint16_t x, uint16_t y) {
  return *((uint8_t*) currentInImage.array + currentInRowbytes * y + x);
}

#endif
