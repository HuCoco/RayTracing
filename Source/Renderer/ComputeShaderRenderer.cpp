#include "ComputeShaderRenderer.h"
#include <fstream>
#include <sstream>
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
    const char* shader_path = "D:/RayTracing/Resource/Shaders/RayTraceBRDF.glsl";
    std::ifstream file;
    file.open(shader_path);
    std::stringstream filestream;
    filestream << file.rdbuf();
    std::string content;
    content = filestream.str();
    mProgram = glCreateProgram();
    mShader = glCreateShader(GL_COMPUTE_SHADER);
    const GLchar* shader_content = content.c_str();
    glShaderSource(mShader, 1, &shader_content, nullptr);
    GLint success;
    GLchar infoLog[512];
    glCompileShader(mShader);
    glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(mShader, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }
    glAttachShader(mProgram, mShader);
    glLinkProgram(mProgram);
    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }
    file.close();
}

void ComputeShaderRenderer::CreateOutputImage(uint32_t width, uint32_t height)
{
    mTargetImageWidth = width;
    mTargetImageHeight = height;
    glGenTextures(1, &mOutputImage);
    glBindTexture(GL_TEXTURE_2D, mOutputImage);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, mTargetImageWidth, mTargetImageHeight);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mOutputImage, 0);
}

void ComputeShaderRenderer::Finalize()
{
    glDeleteProgram(mProgram);
    glDeleteShader(mShader);
    glDeleteTextures(1,&mOutputImage);
}

void ComputeShaderRenderer::Render()
{
   
    glUseProgram(mProgram);
    glFinish();
    auto a = glGetError();
    glBindImageTexture(0, mOutputImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(40, 30, 1);
    glDispatchComputeIndirect(0);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFinish();
}
