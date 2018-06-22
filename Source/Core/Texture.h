#pragma once
#include <Platform/Platform.h>
#include <Core/Color.h>

class Texture
{
public:
    Texture();
    ~Texture();
    void Load(const char* path);
    void Update();
    void SetDimension(uint32_t with, uint32_t height);
    void SetPixel(uint32_t x, uint32_t y, Color color);
    Color GetPixel(float u, float v);
    inline GLuint GetHandle() { return mHandle; }
    inline uint32_t GetWidth() { return mWidth; }
    inline uint32_t GetHeight() { return mHeight; }
private:
    uint32_t mWidth;
    uint32_t mHeight;
    GLuint  mHandle;
    unsigned char* mData;
};