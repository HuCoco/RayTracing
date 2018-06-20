#include "Texture.h"
#include <Math/Math.h>
#include <freeimage.h>
Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Load(const char* path)
{
    FIBITMAP* bitmap = nullptr;
    FREE_IMAGE_FORMAT ImageFormat = FIF_UNKNOWN;
    ImageFormat = FreeImage_GetFileType(path);
    if (ImageFormat == FIF_UNKNOWN)
    {
        ImageFormat = FreeImage_GetFIFFromFilename(path);
    }

    if (ImageFormat == FIF_UNKNOWN)
    {
        return;
    }

    if (FreeImage_FIFSupportsReading(ImageFormat))
    {
        bitmap = FreeImage_Load(ImageFormat, path);
    }

    if (bitmap == nullptr)
    {
        return;
    }

    bitmap = FreeImage_ConvertTo32Bits(bitmap);
    uint32_t Pitch = FreeImage_GetPitch(bitmap);
    mWidth = FreeImage_GetWidth(bitmap);
    mHeight = FreeImage_GetHeight(bitmap);
    mData = new char[mWidth * mHeight * 4];
    BYTE* Bits = FreeImage_GetBits(bitmap);

    for (uint32_t i = 0; i < mHeight; ++i)
    {
        for (uint32_t j = 0; j < mWidth; ++j)
        {
            mData[(i * mWidth + j) * 4 + 0] = 0;//Bits[(i * Pitch) + j * 4 + 2];
            mData[(i * mWidth + j) * 4 + 1] = 0;//Bits[(i * Pitch) + j * 4 + 1];
            mData[(i * mWidth + j) * 4 + 2] = 0;//Bits[(i * Pitch) + j * 4 + 0];
            mData[(i * mWidth + j) * 4 + 3] = 0;//Bits[(i * Pitch) + j * 4 + 3];
        }
    }
}

void Texture::Update()
{

}

void Texture::SetDimension(uint32_t width, uint32_t height)
{

}

void Texture::SetPixel(uint32_t x, uint32_t y, Color color)
{

}

Color Texture::GetPixel(float u, float v)
{
    uint32_t x = u * mWidth;
    uint32_t y = v * mHeight;
    float r = (float)mData[y * mWidth + x + 0] / 255.0f;
    float b = (float)mData[y * mWidth + x + 1] / 255.0f;
    float g = (float)mData[y * mWidth + x + 2] / 255.0f;
    return Color(r, g, b);
}
