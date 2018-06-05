#pragma once
#include <Platform/Platform.h>
#include <Core/Surface.h>
#include <Core/Light.h>
class Scene
{

private:
    std::vector<Surface*> m_surfaces;
    std::vector<Material*> m_materials;
    std::vector<Light*> m_Lights;


    
};