#pragma once
#include <UI/RenderUI.h>
#include <Math/Vec3f.h>
#include <Core/Color.h>

#define MAX_NUM_POINT_LIGHTS 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_SPHERES 32
#define MAX_NUM_PLANES 32
#define MAX_NUM_TRIANGLES 32

class SceneViewer
{
public:
    SceneViewer();
    virtual ~SceneViewer();

    virtual void Initialize();
    virtual void Finalize() = 0;
    virtual void Update() = 0;
    virtual void UpdateShaderData();

protected:
    ViewWindow* m_OutputWindow;
    GLuint mShader;
    GLuint mProgram;

    struct SphereData
    {
        Vec3f center;
        float radius;
        uint32_t mat;
        float _pad[3];
    };

    struct PlaneData
    {
        float A;
        float B;
        float C;
        float D;
        uint32_t mat;
    };

    struct TriangleData
    {
        Vec3f v1;
        float _pad_1;

        Vec3f v2;
        float _pad_2;

        Vec3f v3;
        float _pad_3;

        Vec3f n1;
        float _pad_4;

        Vec3f n2;
        float _pad_5;

        Vec3f n3;
        float _pad_6;

        uint32_t mat;
    };

    struct PointLightData
    {
        Vec3f position;
        float _pad_1;
        Color color;
        float _pad_2;
    };

    struct DirectionLightData
    {
        Color color;
        float _pad_1;
        Vec3f direction;
    };


    struct CameraData
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


    Color mBackgroundColor;

    SphereData mSphereList[MAX_NUM_SPHERES];
    uint32_t mNumActiveSpheres;

    PlaneData mPlaneList[MAX_NUM_PLANES];
    uint32_t mNumActivePlanes;

    TriangleData mTriangleList[MAX_NUM_TRIANGLES];
    uint32_t mNumActiveTriangles;

    PointLightData mPointLightList[MAX_NUM_POINT_LIGHTS];
    uint32_t mNumActivePointLight;

    DirectionLightData mDirectionLight;

    CameraData mCamera;

    GLuint DirectionLightHandle;
    GLuint PointLightsHandle;
    GLuint SpheresHandle;
    GLuint PlanesHandle;
    GLuint TrianglesHandle;
    GLuint CameraHandle;
};