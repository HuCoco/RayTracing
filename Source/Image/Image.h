#pragma once
#include <Platform/Platform.h>

enum class IMAGE_FORMAT
{
    UNKOWN,
    TGA_FORMAT,
};

class Image
{
public:
    Image() = delete;

    static void WriteImage(const char* filename, uint32_t width, uint32_t height, void* data, IMAGE_FORMAT format);

private:
    static void WriteImageTGA(const char* filename, uint32_t width, uint32_t height, void* data);
};