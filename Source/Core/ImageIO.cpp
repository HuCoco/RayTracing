#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <cmath>
#include <fstream>
#include "ImageIO.h"

using namespace std;



/////////////////////////////////////////////////////////////////////////////
// Deallocate the memory allocated to (*imageData) returned by 
// the function ReadImageFile().
// (*imageData) will be set to NULL.
/////////////////////////////////////////////////////////////////////////////

void ImageIO::DeallocateImageData( uchar **imageData )
{
    free( *imageData );
    (*imageData) = NULL;
}





/////////////////////////////////////////////////////////////////////////////
// Read an image from the input filename. 
// Returns 1 if successful or 0 if unsuccessful.
// The returned image data will be pointed to by (*imageData).
// The image width, image height, and number of components (color channels) 
// per pixel will be returned in (*imageWidth), (*imageHeight),
// and (*numComponents).
// The value of (*numComponents) can be 1, 2, 3 or 4.
// The returned image data is always packed tightly with red, green, blue,
// and alpha arranged from lower to higher memory addresses. 
// Each color channel take one byte.
// The first pixel (origin of the image) is at the bottom-left of the image.
/////////////////////////////////////////////////////////////////////////////

int ImageIO::ReadImageFile( const char *filename, uchar **imageData,
                   int *imageWidth, int *imageHeight, int *numComponents,
				   int flags )
{
    return 0;
}





/////////////////////////////////////////////////////////////////////////////
// Save an image to the output filename. 
// Returns 1 if successful or 0 if unsuccessful.
// The input image data is pointed to by imageData.
// The image width, image height, and number of components (color channels) 
// per pixel are provided in imageWidth, imageHeight, numComponents.
// The value of numComponents can be 1, 2, 3 or 4.
// Note that some numComponents cannot be supported by some image file formats. 
// The input image data is assumed packed tightly with red, green, blue,
// and alpha arranged from lower to higher memory addresses. 
// Each color channel take one byte.
// The first pixel (origin of the image) is at the bottom-left of the image.
/////////////////////////////////////////////////////////////////////////////

// Create TGA format image
#pragma pack(push,1)

struct TGA_Header
{
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t img_map_type;
    struct TGA_color_map_spec
    {
        uint16_t first_entry_index;
        uint16_t length;
        uint8_t entry_size;
    } color_map_spec;
    struct TGA_img_spec
    {
        uint16_t x_orig;
        uint16_t y_orig;
        uint16_t width;
        uint16_t height;
        uint8_t pixel_depth;//bits per pixel
        uint8_t img_desc;
    } img_spec;
};
struct TGA_Footer
{
    //TODO: declare footer
};

struct TGA_Extension
{
    //TODO: declare extension
};
#pragma pack(pop)

int ImageIO::SaveImageFile( const char *filename, const uchar *imageData,
                   int imageWidth, int imageHeight, int numComponents,
				   int flags )
{
    char* tga_buffer = new char[sizeof(TGA_Header) + imageWidth * imageHeight * numComponents];
    char* img_buffer = tga_buffer + sizeof(TGA_Header);
    TGA_Header* header = reinterpret_cast<TGA_Header*>(tga_buffer);
    memset(header, 0, sizeof(TGA_Header));
    header->id_length = 0;
    header->color_map_type = 0;
    header->img_map_type = 2;
    header->img_spec.x_orig = 0;
    header->img_spec.y_orig = 0;
    header->img_spec.width = imageWidth;
    header->img_spec.height = imageHeight;
    header->img_spec.pixel_depth = numComponents * 8;
    header->img_spec.img_desc = 0b00001111;

    memcpy(img_buffer, imageData, imageWidth * imageHeight * numComponents);

    std::ofstream outputfile;

    outputfile.open(filename, std::ios_base::out | std::ios_base::binary);

    if (outputfile.is_open())
    {
        outputfile.write(tga_buffer, sizeof(TGA_Header) + imageWidth * imageHeight * numComponents);
        outputfile.close();
    }


    delete tga_buffer;
    return 1;
}
