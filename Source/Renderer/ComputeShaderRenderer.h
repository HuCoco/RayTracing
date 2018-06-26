#pragma once
#include <Platform/Platform.h>

class ComputeShaderRenderer
{
protected:
    ComputeShaderRenderer();
    ~ComputeShaderRenderer();
public:
    static ComputeShaderRenderer* GetInstance();
    void Initialize();
    void CreateOutputImage(uint32_t width, uint32_t height);
    void Finalize();
    void Render();
    GLuint GetOutputImageHandle() { return mOutputImage; }
private:
    GLuint mShader;
    GLuint mProgram;
    GLuint mOutputImage;
    uint32_t mTargetImageWidth;
    uint32_t mTargetImageHeight;
};