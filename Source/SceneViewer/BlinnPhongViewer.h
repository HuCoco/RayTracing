#pragma once
#include <SceneViewer/SceneViewer.h>
#include <Core/Scene.h>

class BlinnPhongSceneViewer : public SceneViewer
{
public:
    BlinnPhongSceneViewer();
    virtual ~BlinnPhongSceneViewer();

    virtual void InitializeScene() = 0;

private:
    Phong::Scene Scene;
};

