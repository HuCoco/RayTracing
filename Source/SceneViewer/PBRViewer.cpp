#include "PBRViewer.h"
#include <Core/Sphere.h>
#include <Core/Plane.h>
#include <Core/Triangle.h>
#include <Core/Surface.h>
#include <fstream>
#include <sstream>
PBRSceneViewer::PBRSceneViewer()
{

}

PBRSceneViewer::~PBRSceneViewer()
{

}

void PBRSceneViewer::Initialize()
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

    SceneViewer::Initialize();
    glGenBuffers(1, &mMaterialsHandle);
}

void PBRSceneViewer::Finalize()
{

}

void PBRSceneViewer::SetScene(void* scene)
{
    m_Scene = reinterpret_cast<PBR::Scene*>(scene);
}

void PBRSceneViewer::Update()
{
    SetupShaderData();
    UpdateShaderData();
    DoRender();
}

void PBRSceneViewer::SetupShaderData()
{
    mDirectionLight.color = m_Scene->amLight.I_a;
    mDirectionLight.direction.x = (float)m_Scene->amLight.direction.x();
    mDirectionLight.direction.y = (float)m_Scene->amLight.direction.y();
    mDirectionLight.direction.z = (float)m_Scene->amLight.direction.z();

    mNumActivePointLight = m_Scene->numPtLights < 32 ? m_Scene->numPtLights : 32;
    for (uint32_t i = 0; i < mNumActivePointLight; ++i)
    {
        mPointLightList[i].position.x = (float)m_Scene->ptLight[i].position.x();
        mPointLightList[i].position.y = (float)m_Scene->ptLight[i].position.y();
        mPointLightList[i].position.z = (float)m_Scene->ptLight[i].position.z();
        mPointLightList[i].color = m_Scene->ptLight[i].I_source;
    }

    mNumActiveMaterials = m_Scene->numMaterials < 32 ? m_Scene->numMaterials : 32;
    for (uint32_t i = 0; i < mNumActiveMaterials; ++i)
    {
        mMaterialList[i].albedo = (m_Scene->material)[i].albedo;
        mMaterialList[i].metallic = (m_Scene->material)[i].metallic;
        mMaterialList[i].roughness = (m_Scene->material)[i].roughness;
    }

    int numSurface = m_Scene->numSurfaces;
    mNumActiveSpheres = 0;
    mNumActivePlanes = 0;
    mNumActiveTriangles = 0;
    for (int i = 0; i < numSurface; ++i)
    {
        switch (m_Scene->surfacep[i]->type)
        {
        case Surface::SurfaceType::SPHERER_SURFACE:
        {
            mSphereList[mNumActiveSpheres].center.x = reinterpret_cast<Sphere*>(m_Scene->surfacep[i])->center.x();
            mSphereList[mNumActiveSpheres].center.y = reinterpret_cast<Sphere*>(m_Scene->surfacep[i])->center.y();
            mSphereList[mNumActiveSpheres].center.z = reinterpret_cast<Sphere*>(m_Scene->surfacep[i])->center.z();
            mSphereList[mNumActiveSpheres].radius = reinterpret_cast<Sphere*>(m_Scene->surfacep[i])->radius;
            mSphereList[mNumActiveSpheres].mat = reinterpret_cast<Sphere*>(m_Scene->surfacep[i])->mat;
            mNumActiveSpheres += 1;
            break;
        }
        case Surface::SurfaceType::PLANE_SURFACE:
        {
            mPlaneList[mNumActivePlanes].A = reinterpret_cast<Plane*>(m_Scene->surfacep[i])->A;
            mPlaneList[mNumActivePlanes].B = reinterpret_cast<Plane*>(m_Scene->surfacep[i])->B;
            mPlaneList[mNumActivePlanes].C = reinterpret_cast<Plane*>(m_Scene->surfacep[i])->C;
            mPlaneList[mNumActivePlanes].D = reinterpret_cast<Plane*>(m_Scene->surfacep[i])->D;
            mPlaneList[mNumActivePlanes].mat = reinterpret_cast<Plane*>(m_Scene->surfacep[i])->mat;;
            mNumActivePlanes += 1;
            break;
        }
        case Surface::SurfaceType::TRIANGLE_SURFACE:
        {
            mTriangleList[mNumActiveTriangles].v1.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v0.x();
            mTriangleList[mNumActiveTriangles].v1.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v0.y();
            mTriangleList[mNumActiveTriangles].v1.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v0.z();

            mTriangleList[mNumActiveTriangles].v2.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v1.x();
            mTriangleList[mNumActiveTriangles].v2.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v1.y();
            mTriangleList[mNumActiveTriangles].v2.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v1.z();

            mTriangleList[mNumActiveTriangles].v3.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v2.x();
            mTriangleList[mNumActiveTriangles].v3.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v2.y();
            mTriangleList[mNumActiveTriangles].v3.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->v2.z();

            mTriangleList[mNumActiveTriangles].n1.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n0.x();
            mTriangleList[mNumActiveTriangles].n1.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n0.y();
            mTriangleList[mNumActiveTriangles].n1.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n0.z();

            mTriangleList[mNumActiveTriangles].n2.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n1.x();
            mTriangleList[mNumActiveTriangles].n2.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n1.y();
            mTriangleList[mNumActiveTriangles].n2.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n1.z();

            mTriangleList[mNumActiveTriangles].n3.x = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n2.x();
            mTriangleList[mNumActiveTriangles].n3.y = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n2.y();
            mTriangleList[mNumActiveTriangles].n3.z = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->n2.z();

            mTriangleList[mNumActiveTriangles].mat = reinterpret_cast<Triangle*>(m_Scene->surfacep[i])->mat;
            mNumActiveTriangles += 1;
            break;
        }
        default:
            break;
        }
    }

    mCamera.COP.x = m_Scene->camera.mCOP.x();
    mCamera.COP.y = m_Scene->camera.mCOP.y();
    mCamera.COP.z = m_Scene->camera.mCOP.z();

    mCamera.ImageOrigin.x = m_Scene->camera.mImageOrigin.x();
    mCamera.ImageOrigin.y = m_Scene->camera.mImageOrigin.y();
    mCamera.ImageOrigin.z = m_Scene->camera.mImageOrigin.z();

    mCamera.ImageU.x = m_Scene->camera.mImageU.x();
    mCamera.ImageU.y = m_Scene->camera.mImageU.y();
    mCamera.ImageU.z = m_Scene->camera.mImageU.z();

    mCamera.ImageV.x = m_Scene->camera.mImageV.x();
    mCamera.ImageV.y = m_Scene->camera.mImageV.y();
    mCamera.ImageV.z = m_Scene->camera.mImageV.z();

    mCamera.ImageWidth = m_Scene->camera.getImageWidth();
    mCamera.ImageHeight = m_Scene->camera.getImageHeight();
}

void PBRSceneViewer::UpdateShaderData()
{
    SceneViewer::UpdateShaderData();
    glBindBuffer(GL_UNIFORM_BUFFER, mMaterialsHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData) * MAX_NUM_POINT_LIGHTS, mMaterialList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, mMaterialsHandle);
   
    glUniform1i(glGetUniformLocation(mProgram, "NumActiveMaterials"), mNumActiveMaterials);
}
