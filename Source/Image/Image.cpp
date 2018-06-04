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

struct TGAFileHeader
{
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t image_type;

    struct 
    {
        uint16_t color_map_address;
        uint16_t color_map_count;
        uint8_t color_map_element_size;
    } color_map_specification;

    struct 
    {
        uint16_t origin_x;
        uint16_t origin_y;
        uint16_t width;
        uint16_t height;
        uint8_t depth;
        uint8_t desc;
    } image_specification;
};


struct TGAFileFooter
{
    uint32_t a;
    uint32_t b;
    char signal[16];
    const char end = '.';
    const char eof = '\0';
};
void Image::WriteImageTGA(const char* filename, uint32_t width, uint32_t height, void* data)
{
    std::ofstream file(filename);
    TGAFileHeader file_content_header;
    file_content_header.id_length = 0;
    file_content_header.color_map_type = 0;
    file_content_header.image_type = 1;
    file_content_header.color_map_specification.color_map_address = 0;
    file_content_header.color_map_specification.color_map_count = 0;
    file_content_header.color_map_specification.color_map_element_size = 0;
    file_content_header.image_specification.origin_x = 0;
    file_content_header.image_specification.origin_y = 0;
    file_content_header.image_specification.width = width;
    file_content_header.image_specification.height = height;
    file_content_header.image_specification.depth = 32;
    file_content_header.image_specification.desc |= 0b101000;

    TGAFileFooter file_content_footer;
    file_content_footer.a = 0;
    file_content_footer.b = 0;

    size_t file_size = sizeof(TGAFileHeader) + (width * height * 4) + sizeof(TGAFileFooter);
    char* image_data = new char[file_size];

    memcpy(image_data, &file_content_header, sizeof(TGAFileHeader));
    memcpy(image_data + sizeof(TGAFileHeader), data, width * height * 4);
    memcpy(image_data + sizeof(TGAFileHeader) + width * height * 4, &file_content_footer, sizeof(TGAFileFooter));
    file.write(image_data, file_size);
    file.close();
}

