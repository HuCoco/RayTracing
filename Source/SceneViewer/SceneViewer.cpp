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
    glBindBufferBase(GL_UNIFORM_BUFFER, 4, SpheresHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 5, SpheresHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 6, CameraHandle);

    glGetUniformLocation(mProgram, "NumActiveSpheres");
    glUniform1ui(glGetUniformLocation(mProgram, "NumActiveLights"), mNumActivePointLight);
    glUniform1ui(glGetUniformLocation(mProgram, "NumActiveSpheres"), mNumActiveSpheres);
    glUniform1ui(glGetUniformLocation(mProgram, "NumSample"), 100);
    glUniform1ui(glGetUniformLocation(mProgram, "ReflectLevels"), 1);
    glUniform1i(glGetUniformLocation(mProgram, "HasShadow"), 0);
}

