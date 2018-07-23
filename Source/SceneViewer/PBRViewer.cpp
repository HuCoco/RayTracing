#include "PBRViewer.h"
#include <Core/Sphere.h>
#include <Core/Plane.h>
#include <Core/Triangle.h>
#include <Core/Surface.h>
PBRSceneViewer::PBRSceneViewer()
{

}

PBRSceneViewer::~PBRSceneViewer()
{

}

void PBRSceneViewer::Initialize()
{


    SceneViewer::Initialize();

    glGenBuffers(1, &mMaterialsHandle);

    InitializeScene();
}

void PBRSceneViewer::Update()
{
    
}

void PBRSceneViewer::SetupShaderData()
{
    mDirectionLight.color = m_Scene.amLight.I_a;
    mDirectionLight.direction.x = (float)m_Scene.amLight.direction.x();
    mDirectionLight.direction.y = (float)m_Scene.amLight.direction.y();
    mDirectionLight.direction.z = (float)m_Scene.amLight.direction.z();

    mNumActivePointLight = m_Scene.numPtLights < 32 ? m_Scene.numPtLights : 32;
    for (uint32_t i = 0; i < mNumActivePointLight; ++i)
    {
        mPointLightList[i].position.x = (float)m_Scene.ptLight[i].position.x();
        mPointLightList[i].position.y = (float)m_Scene.ptLight[i].position.y();
        mPointLightList[i].position.z = (float)m_Scene.ptLight[i].position.z();
        mPointLightList[i].color = m_Scene.ptLight[i].I_source;
    }

    mNumActiveMaterials = m_Scene.numMaterials < 32 ? m_Scene.numMaterials : 32;
    for (uint32_t i = 0; i < mNumActiveMaterials; ++i)
    {
        mMaterialList[i].albedo = (m_Scene.material)[i].albedo;
        mMaterialList[i].metallic = (m_Scene.material)[i].metallic;
        mMaterialList[i].roughness = (m_Scene.material)[i].roughness;
    }

    int numSurface = m_Scene.numSurfaces;
    for (int i = 0; i < numSurface; ++i)
    {
        switch (m_Scene.surfacep[i]->type)
        {
        case Surface::SurfaceType::SPHERER_SURFACE:
        {
            mSphereList[mNumActiveSpheres].center.x = reinterpret_cast<Sphere*>(m_Scene.surfacep[i])->center.x();
            mSphereList[mNumActiveSpheres].center.y = reinterpret_cast<Sphere*>(m_Scene.surfacep[i])->center.y();
            mSphereList[mNumActiveSpheres].center.z = reinterpret_cast<Sphere*>(m_Scene.surfacep[i])->center.z();
            mSphereList[mNumActiveSpheres].radius = reinterpret_cast<Sphere*>(m_Scene.surfacep[i])->radius;
            mSphereList[mNumActiveSpheres].mat = i;
            mNumActiveSpheres += 1;
            break;
        }
        case Surface::SurfaceType::PLANE_SURFACE:
        {
            mPlaneList[mNumActivePlanes].A = reinterpret_cast<Plane*>(m_Scene.surfacep[i])->A;
            mPlaneList[mNumActivePlanes].B = reinterpret_cast<Plane*>(m_Scene.surfacep[i])->B;
            mPlaneList[mNumActivePlanes].C = reinterpret_cast<Plane*>(m_Scene.surfacep[i])->C;
            mPlaneList[mNumActivePlanes].D = reinterpret_cast<Plane*>(m_Scene.surfacep[i])->D;
            mPlaneList[mNumActivePlanes].mat = i;
            mNumActivePlanes += 1;
            break;
        }
        case Surface::SurfaceType::TRIANGLE_SURFACE:
        {
            mTriangleList[mNumActiveSpheres].v1.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v0.x();
            mTriangleList[mNumActiveSpheres].v1.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v0.y();
            mTriangleList[mNumActiveSpheres].v1.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v0.z();

            mTriangleList[mNumActiveSpheres].v2.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v1.x();
            mTriangleList[mNumActiveSpheres].v2.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v1.y();
            mTriangleList[mNumActiveSpheres].v2.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v1.z();

            mTriangleList[mNumActiveSpheres].v3.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v2.x();
            mTriangleList[mNumActiveSpheres].v3.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v2.y();
            mTriangleList[mNumActiveSpheres].v3.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->v2.z();

            mTriangleList[mNumActiveSpheres].n1.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n0.x();
            mTriangleList[mNumActiveSpheres].n1.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n0.y();
            mTriangleList[mNumActiveSpheres].n1.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n0.z();

            mTriangleList[mNumActiveSpheres].n2.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n1.x();
            mTriangleList[mNumActiveSpheres].n2.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n1.y();
            mTriangleList[mNumActiveSpheres].n2.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n1.z();

            mTriangleList[mNumActiveSpheres].n3.x = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n2.x();
            mTriangleList[mNumActiveSpheres].n3.y = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n2.y();
            mTriangleList[mNumActiveSpheres].n3.z = reinterpret_cast<Triangle*>(m_Scene.surfacep[i])->n2.z();
            break;
        }
        default:
            break;
        }
    }

    mCamera.COP.x = m_Scene.camera.mCOP.x();
    mCamera.COP.y = m_Scene.camera.mCOP.y();
    mCamera.COP.z = m_Scene.camera.mCOP.z();

    mCamera.ImageOrigin.x = m_Scene.camera.mImageOrigin.x();
    mCamera.ImageOrigin.y = m_Scene.camera.mImageOrigin.y();
    mCamera.ImageOrigin.z = m_Scene.camera.mImageOrigin.z();

    mCamera.ImageU.x = m_Scene.camera.mImageU.x();
    mCamera.ImageU.y = m_Scene.camera.mImageU.y();
    mCamera.ImageU.z = m_Scene.camera.mImageU.z();

    mCamera.ImageV.x = m_Scene.camera.mImageV.x();
    mCamera.ImageV.y = m_Scene.camera.mImageV.y();
    mCamera.ImageV.z = m_Scene.camera.mImageV.z();

    mCamera.ImageWidth = m_Scene.camera.getImageWidth();
    mCamera.ImageHeight = m_Scene.camera.getImageHeight();
}

void PBRSceneViewer::UpdateShaderData()
{
    glBindBuffer(GL_UNIFORM_BUFFER, mMaterialsHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData) * MAX_NUM_POINT_LIGHTS, mMaterialList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, mMaterialsHandle);

    glUniform1i(glGetUniformLocation(mProgram, "NumActiveMaterials"), mNumActiveMaterials);
}

PBRMaterialBallViewer::PBRMaterialBallViewer()
{

}

PBRMaterialBallViewer::~PBRMaterialBallViewer()
{

}

void PBRMaterialBallViewer::InitializeScene()
{
    using namespace PBR;
    m_Scene.backgroundColor = Color(0.0f, 0.0f, 0.0f);

    m_Scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.55f;
    m_Scene.amLight.direction = Vector3d(0.0f, 1.0f, 0.0f);

    m_Scene.numMaterials = 1;
    m_Scene.material = new Material[m_Scene.numMaterials];
    m_Scene.material[0].albedo = Color(0.0f, 0.0f, 0.0f);
    m_Scene.material[0].metallic = 0.001;
    m_Scene.material[0].roughness = 0.001;

    m_Scene.numSurfaces = 1;
    m_Scene.surfacep = new SurfacePtr[m_Scene.numSurfaces];
    m_Scene.surfacep[0] = new Sphere(Vector3d(35.0, 25.0, 100.0), 5.0, &m_Scene.material[0]);

    m_Scene.numPtLights = 2;
    m_Scene.ptLight = new PointLightSource[m_Scene.numPtLights];

    m_Scene.ptLight[0].I_source = Color(300.0, 300.0, 300.0);
    m_Scene.ptLight[0].position = Vector3d(30.0, 40.0, 80.0);

    m_Scene.ptLight[1].I_source = Color(300.0, 300.0, 300.0);
    m_Scene.ptLight[1].position = Vector3d(-30.0, 40.0, 80.0);


    float imageWidth = (float)m_OutputWindow->GetWidth();
    float imageHeight = (float)m_OutputWindow->GetHeight();
    m_Scene.camera = Camera(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 10.0), Vector3d(0.0, 1.0, 0.0),
        (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
        imageWidth, imageHeight);;
}

void PBRMaterialBallViewer::Initialize()
{
    PBRSceneViewer::Initialize();

}

void PBRMaterialBallViewer::Finalize()
{

}

void PBRMaterialBallViewer::UpdateShaderData()
{
    PBRSceneViewer::UpdateShaderData();
}
