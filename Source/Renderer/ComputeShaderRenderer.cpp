#include "ComputeShaderRenderer.h"
#include <fstream>
#include <sstream>
#include <Core/Sphere.h>

namespace PBR
{
#define MAX_NUM_POINT_LIGHTS 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_SPHERES 32

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
        glDeleteTextures(1, &mOutputImage);
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

    void ComputeShaderRenderer::PrepareShaderData()
    {
        glGenBuffers(1, &DirectionLightHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, DirectionLightHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionLight), &mDirectionLight, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &PointLightsHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, PointLightsHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * MAX_NUM_POINT_LIGHTS, mPointLightList, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &MaterialsHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, MaterialsHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Material) * MAX_NUM_POINT_LIGHTS, mMaterialList, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &SpheresHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, SpheresHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_NUM_SPHERES, mSphereList, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &CameraHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, CameraHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Camera), &mCamera, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void ComputeShaderRenderer::PassShaderData()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, PointLightsHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * MAX_NUM_POINT_LIGHTS, mPointLightList, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);


        glBindBufferBase(GL_UNIFORM_BUFFER, 0, DirectionLightHandle);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, PointLightsHandle);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MaterialsHandle);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, SpheresHandle);
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, CameraHandle);

        auto a = glGetUniformLocation(mProgram, "NumActiveSpheres");
        glUniform1ui(glGetUniformLocation(mProgram, "NumActiveLights"), mNumActivePointLight);
        //glUniform1i(glGetUniformLocation(mProgram, "NumActiveMaterials"), mNumActiveMaterials);
        glUniform1ui(glGetUniformLocation(mProgram, "NumActiveSpheres"), mNumActiveSpheres);
        glUniform1ui(glGetUniformLocation(mProgram, "NumSample"), 100);
        glUniform1ui(glGetUniformLocation(mProgram, "ReflectLevels"), 1);
        glUniform1i(glGetUniformLocation(mProgram, "HasShadow"), 0);
    }

    void ComputeShaderRenderer::GenerateShaderData(const Scene& scene)
    {
        mDirectionLight.color = scene.amLight.I_a;
        static float a = 0.01f;
        static float step = 0.1;
        static float range = 10;
        a += step;


        mNumActivePointLight = scene.numPtLights < 32 ? scene.numPtLights : 32;
        for (uint32_t i = 0; i < mNumActivePointLight; ++i)
        {
            float xx;
            float yy;
            if (i % 2 == 0)
            {
                xx = sin(a)*range;
                yy = cos(a)*range;

            }
            else
            {
                xx = -sin(a)*range;
                yy = -cos(a)*range;
            }
            mPointLightList[i].position.x = (float)scene.ptLight[i].position.x() + xx;
            mPointLightList[i].position.y = (float)scene.ptLight[i].position.y() + yy;
            mPointLightList[i].position.z = (float)scene.ptLight[i].position.z() + yy;
            mPointLightList[i].color = scene.ptLight[i].I_source;
        }

        mNumActiveMaterials = scene.numMaterials < 32 ? scene.numMaterials : 32;
        for (uint32_t i = 0; i < mNumActiveMaterials; ++i)
        {
            mMaterialList[i].albedo = (scene.material)[i].albedo;
            mMaterialList[i].metallic = (scene.material)[i].metallic;
            mMaterialList[i].roughness = (scene.material)[i].roughness;
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


        mCamera.COP.x = scene.camera.mCOP.x();
        mCamera.COP.y = scene.camera.mCOP.y();
        mCamera.COP.z = scene.camera.mCOP.z();

        mCamera.ImageOrigin.x = scene.camera.mImageOrigin.x();
        mCamera.ImageOrigin.y = scene.camera.mImageOrigin.y();
        mCamera.ImageOrigin.z = scene.camera.mImageOrigin.z();

        mCamera.ImageU.x = scene.camera.mImageU.x();
        mCamera.ImageU.y = scene.camera.mImageU.y();
        mCamera.ImageU.z = scene.camera.mImageU.z();

        mCamera.ImageV.x = scene.camera.mImageV.x();
        mCamera.ImageV.y = scene.camera.mImageV.y();
        mCamera.ImageV.z = scene.camera.mImageV.z();

        mCamera.ImageWidth = scene.camera.getImageWidth();
        mCamera.ImageHeight = scene.camera.getImageHeight();
    }

}