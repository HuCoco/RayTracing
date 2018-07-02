#pragma once
#include <Platform/Platform.h>
#include <Math/Vec3f.h>
#include <Core/Scene.h>
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
    void PrepareShaderData();
    void PassShaderData();
    void GenerateShaderData(const Scene& scene);
    GLuint GetOutputImageHandle() { return mOutputImage; }
private:
    GLuint mShader;
    GLuint mProgram;
    GLuint mOutputImage;
    uint32_t mTargetImageWidth;
    uint32_t mTargetImageHeight;

    //Direction Light 
    GLuint DirectionLightHandle;
    GLuint PointLightsHandle;
    GLuint MaterialsHandle;
    GLuint SpheresHandle;
    GLuint CameraHandle;

private:
    struct Material
    {
        Color albedo;
        float metallic;
        float roughness;
    };

    Material mMaterialList[32];
    uint32_t mNumActiveMaterials;
     
    struct Sphere
    {
        Vec3f center;
        float radius;
        uint32_t mat;
    };

    Sphere mSphereList[32];
    uint32_t mNumActiveSpheres;

    struct PointLight
    {
        Vec3f position;
        Color color;
    };

    PointLight mPointLightList[32];
    uint32_t mNumActivePointLight;

    struct DirectionLight
    {
        Color color;
    };

    DirectionLight mDirectionLight;

    struct Camera
    {
        Vec3f COP;
        Vec3f ImageOrigin;
        Vec3f ImageU;
        Vec3f ImageV;
        int ImageWidth;
        int ImageHeight;
    };

    Camera mCamera;
};