#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include "Color.h"
#include "Ray.h"
#include "Scene.h"

namespace Phong
{
    class Raytrace
    {
    public:
        static Color TraceRay(const Ray &ray, const Scene &scene,
            int reflectLevels, bool hasShadow);
    };

}

namespace PBR
{
    class Raytrace
    {
    public:
        static Color TraceRay(const Ray &ray, const Scene &scene,
            int reflectLevels, bool hasShadow);
    };

}


#endif // _RAYTRACE_H_
