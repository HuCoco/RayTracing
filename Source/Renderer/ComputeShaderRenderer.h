#pragma once
#include <Platform/Platform.h>
#include <Math/Vec3f.h>
#include <Core/Scene.h>

namespace PBR
{
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
            float _pad[3];
        };

        Material mMaterialList[32];
        uint32_t mNumActiveMaterials;

        struct Sphere
        {
            Vec3f center;
            float radius;
            uint32_t mat;
            float _pad[3];
        };

        Sphere mSphereList[32];
        uint32_t mNumActiveSpheres;

        struct PointLight
        {
            Vec3f position;
            float _pad_1;
            Color color;
            float _pad_2;
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
            float _pad_1;
            Vec3f ImageOrigin;
            float _pad_2;
            Vec3f ImageU;
            float _pad_3;
            Vec3f ImageV;
            int ImageWidth;
            int ImageHeight;
        };

        Camera mCamera;
    };
}