#include "ComputeShaderRenderer.h"
#include <fstream>
#include <sstream>
#include <Core/Sphere.h>
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

void ComputeShaderRenderer::PassShaderData()
{

}

void ComputeShaderRenderer::GenerateShaderData(const Scene& scene)
{
    mDirectionLight.color = scene.amLight.I_a;

    mNumActivePointLight = scene.numPtLights < 32 ? scene.numPtLights : 32;
    for (uint32_t i = 0; i < mNumActivePointLight; ++i)
    {
        mPointLightList[i].position.x = (float)scene.ptLight[i].position.x();
        mPointLightList[i].position.y = (float)scene.ptLight[i].position.y();
        mPointLightList[i].position.z = (float)scene.ptLight[i].position.z();
        mPointLightList[i].color = scene.ptLight[i].I_source;
    }

    mNumActiveMaterials = scene.numMaterials < 32 ? scene.numMaterials : 32;
    for (uint32_t i = 0; i < mNumActiveMaterials; ++i)
    {
        mMaterialList[i].albedo = reinterpret_cast<PBRMaterial*>(scene.numMaterials)[i].albedo;
        mMaterialList[i].metallic = reinterpret_cast<PBRMaterial*>(scene.numMaterials)[i].metallic;
        mMaterialList[i].roughness = reinterpret_cast<PBRMaterial*>(scene.numMaterials)[i].roughness;
    }

    mNumActiveSpheres = scene.numSurfaces < 32 ? scene.numSurfaces : 32;
    for (uint32_t i = 0; i < mNumActiveSpheres; ++i)
    {
        mSphereList[i].center.x = reinterpret_cast<::Sphere*>(scene.surfacep[i])->center.x();
        mSphereList[i].center.y = reinterpret_cast<::Sphere*>(scene.surfacep[i])->center.y();
        mSphereList[i].center.z = reinterpret_cast<::Sphere*>(scene.surfacep[i])->center.z();
        mSphereList[i].radius = reinterpret_cast<::Sphere*>(scene.surfacep[i])->radius;
        mSphereList[i].mat = i;
    }


}
