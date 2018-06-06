#pragma once
#include <Core/Scene.h>
#include <Core/Ray.h>
#include <Math/Color.h>

class RayTracer
{
public:
    static Vec3f TraceRay(const Ray& ray, const Scene& scene, uint32_t numRefection);
};