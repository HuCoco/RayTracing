#pragma once
#include <Platform/Platform.h>
#include <Math/Color.h>
#include <Math/Vec3f.h>
enum class IMAGE_FORMAT
{
    UNKOWN,
    TGA_FORMAT,
};

class Image
{
public:
    Image(uint32_t w, uint32_t h);
    void SetPixel(uint32_t x, uint32_t y, Vec3f data);
    inline void* GetData() { return mData; }
    static void WriteImage(const char* filename, uint32_t width, uint32_t height, void* data, IMAGE_FORMAT format);
private:
    static void WriteImageTGA(const char* filename, uint32_t width, uint32_t height, void* data);
    
private:
    uint32_t mWidth;
    uint32_t mHeight;
    UColor* mData;
};