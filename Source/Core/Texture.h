#pragma once
#include <Platform/Platform.h>
#include <Core/Color.h>

class Texture
{
public:
    Texture();
    ~Texture();

    void Update();
    void SetDimension(uint32_t with, uint32_t height);
    void SetPixel(uint32_t x, uint32_t y, Color color);
    inline GLuint GetHandle() { return mHandle; }
private:
    uint32_t mWidth;
    uint32_t mHeight;
    GLuint  mHandle;
    Color* mData;
};