#include "SceneViewer.h"

SceneViewer::SceneViewer()
{

}

SceneViewer::~SceneViewer()
{

}

void SceneViewer::Initialize()
{
    glGenBuffers(1, &DirectionLightHandle);
    glGenBuffers(1, &PointLightsHandle);
    glGenBuffers(1, &SpheresHandle);
    glGenBuffers(1, &PlanesHandle);
    glGenBuffers(1, &TrianglesHandle);
    glGenBuffers(1, &CameraHandle);
}

void SceneViewer::UpdateShaderData()
{
    glBindBuffer(GL_UNIFORM_BUFFER, DirectionLightHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionLightData), &mDirectionLight, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, PointLightsHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightData) * MAX_NUM_POINT_LIGHTS, mPointLightList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, SpheresHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SphereData) * MAX_NUM_SPHERES, mSphereList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, PlanesHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PlaneData) * MAX_NUM_PLANES, mPlaneList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, TrianglesHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(TriangleData) * MAX_NUM_TRIANGLES, mTriangleList, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, CameraHandle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &mCamera, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, DirectionLightHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, PointLightsHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, SpheresHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 4, PlanesHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 5, TrianglesHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 6, CameraHandle);
    

    glUniform1ui(glGetUniformLocation(mProgram, "NumActiveLights"), mNumActivePointLight);
    glUniform1ui(glGetUniformLocation(mProgram, "NumActiveSpheres"), mNumActiveSpheres);
    glUniform1ui(glGetUniformLocation(mProgram, "mNumActivePlanes"), mNumActivePlanes);
    glUniform1ui(glGetUniformLocation(mProgram, "mNumActiveTriangles"), mNumActiveTriangles);
    glUniform1ui(glGetUniformLocation(mProgram, "NumSample"), 100);
    glUniform1ui(glGetUniformLocation(mProgram, "ReflectLevels"), 0);
    glUniform1i(glGetUniformLocation(mProgram, "HasShadow"),1);
        

}

void SceneViewer::SetViewWindow(ViewWindow* output)
{
    m_OutputWindow = output;
    glGenTextures(1, &mOutputTexture);
    glBindTexture(GL_TEXTURE_2D, mOutputTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_OutputWindow->GetWidth(), m_OutputWindow->GetHeight());
    m_OutputWindow->SetImTextureID((ImTextureID)(mOutputTexture));
}

void SceneViewer::DoRender()
{
    glUseProgram(mProgram);
    glFinish();
    glBindImageTexture(0, mOutputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(40, 30, 1);
    glDispatchComputeIndirect(0);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFinish();

    m_OutputWindow->Update();
}

