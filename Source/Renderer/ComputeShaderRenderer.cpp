#include "ComputeShaderRenderer.h"

ComputeShaderRenderer::ComputeShaderRenderer()
{

}

ComputeShaderRenderer::~ComputeShaderRenderer()
{

}

ComputeShaderRenderer* ComputeShaderRenderer::GetInstance()
{
    static ComputeShaderRenderer sInstance;
    return &sInstance;
}

void ComputeShaderRenderer::Initialize()
{

}

void ComputeShaderRenderer::Finalize()
{

}
