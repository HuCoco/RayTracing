#pragma once
#include <Image/Image.h>

int main()
{
    char* data = new char[128 * 128 * 4];
    Image::WriteImage("C:/Users/Huke/Desktop/ImageTest/test.tga", 128, 128, data, IMAGE_FORMAT::TGA_FORMAT);
    return 0;
}