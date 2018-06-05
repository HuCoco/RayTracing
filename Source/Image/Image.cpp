#include "Image.h"
#include <fstream>


void Image::WriteImage(const char* filename, uint32_t width, uint32_t height, void* data, IMAGE_FORMAT format)
{
    switch (format)
    {
    case IMAGE_FORMAT::UNKOWN:
        break;
    case IMAGE_FORMAT::TGA_FORMAT:
        Image::WriteImageTGA(filename, width, height, data);
        break;
    default:
        break;
    }
}


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

void Image::WriteImageTGA(const char* filename, uint32_t width, uint32_t height, void* data)
{
    int numComponents = 4;
    char* tga_buffer = new char[sizeof(TGA_Header) + width * height * numComponents];
    char* img_buffer = tga_buffer + sizeof(TGA_Header);
    size_t a = sizeof(TGA_Header);
    TGA_Header* header = reinterpret_cast<TGA_Header*>(tga_buffer);
    memset(header, 0, sizeof(TGA_Header));
    header->id_length = 0;
    header->color_map_type = 0;
    header->img_map_type = 2;
    header->img_spec.x_orig = 0;
    header->img_spec.y_orig = 0;
    header->img_spec.width = width;
    header->img_spec.height = height;
    header->img_spec.pixel_depth = numComponents * 8;
    header->img_spec.img_desc = 0b00001111;
    memcpy(img_buffer, data, width * height * numComponents);

    std::ofstream outputfile;

    outputfile.open(filename, std::ios_base::out | std::ios_base::binary);

    if (outputfile.is_open())
    {
        outputfile.write(tga_buffer, sizeof(TGA_Header) + width * height * numComponents);
        outputfile.close();
    }


    delete tga_buffer;
}

