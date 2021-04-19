#include "inputdevice.h"
#include "ivm_io/ivm_io.h"

#include <stdio.h>

DBOOL get_image(boxing_image8** image, unsigned frame)
{
    printf("reading image %u\n",frame);

    long width;
    long height;
    ivm64_read_frame(&width, &height, frame);
    if (width == 0 || height == 0)
    {
        printf("Illegal image size: %ldx%ld\n", width, height);
        return DFALSE;
    }
    
    *image = boxing_image8_create(width, height);
    if (*image == NULL)
    {
        printf("Failed to allocate image with size: %ldx%ld\n", width, height);
        return DFALSE;
    }

    boxing_image8_pixel * data = (*image)->data;
    for (long y = 0; y < height; y++)
    {
        for (long x = 0; x < width; x++)
        {
            uint8_t pixel = ivm64_read_pixel(x, y);
            
            *data = (boxing_image8_pixel)(pixel);
            data++;
        }
    }

    printf("reading image %u complete\n",frame);
    return DTRUE;
}
