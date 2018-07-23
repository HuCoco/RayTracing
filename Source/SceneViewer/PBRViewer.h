#pragma once
#include <SceneViewer/SceneViewer.h>
#include <Core/Scene.h>
#include <Math/Vec3f.h>

class PBRSceneViewer : public SceneViewer
{
public:
    PBRSceneViewer();
    virtual ~PBRSceneViewer();

    virtual void Initialize();
    virtual void InitializeScene() = 0;
    virtual void Update();
    void SetupShaderData();
    virtual void UpdateShaderData();
protected:
    PBR::Scene m_Scene;

    struct MaterialData
    {
        Color albedo;
        float metallic;
        float roughness;
        float _pad[3];
    };


    MaterialData mMaterialList[32];
    uint32_t mNumActiveMaterials;
    GLuint mMaterialsHandle;
};




class PBRMaterialBallViewer : public PBRSceneViewer
{
public:
    PBRMaterialBallViewer();
    virtual ~PBRMaterialBallViewer();

    virtual void InitializeScene();
    virtual void Initialize();
    virtual void Finalize();
    virtual void UpdateShaderData();
};