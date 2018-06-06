#pragma once
#include <Platform/Platform.h>
#include <Core/Surface.h>
#include <Core/Light.h>
#include <Core/Camera.h>
class Scene
{

public:
    std::vector<Surface*> m_surfaces;
    std::vector<Material> m_materials;
    std::vector<Light> m_Lights;

    Camera camera;
  
};