#include "Texture.h"
#include <Math/Math.h>
Texture::Texture()
{
    glGenTextures(1, &mHandle);
    mData = nullptr;
}

Texture::~Texture()
{
    glDeleteTextures(1, &mHandle);
}

void Texture::Update()
{
    glBindTexture(GL_TEXTURE_2D, mHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, mData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetDimension(uint32_t with, uint32_t height)
{
    if (mData == nullptr)
    {
        mWidth = with;
        mHeight = height;
        mData = new Color[mWidth * mHeight];
    }
}

void Texture::SetPixel(uint32_t x, uint32_t y, Color color)
{
    //mData[(y ) * mWidth + x] = color;
    mData[(mHeight - y - 1) * mWidth + x] = color;

    //uint32_t px = x;
    //uint32_t py = (mHeight - 1) - y;
    //uint32_t index = py * mWidth + px;
    //auto color_convert = [](float v)
    //{
    //    uint8_t ret = 0;
    //    ret = v * 255;
    //    Math::clamp(ret, (uint8_t)0, (uint8_t)255);
    //    return ret;
    //};

    //mData[index] = color;
}
