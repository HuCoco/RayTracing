#pragma once
#include <Platform/Platform.h>
#include <Math/Vec3f.h>
#include <Core/Material.h>
#include <Core/Ray.h>

struct SurfaceHitResult
{
    float t;
    Vec3f position;
    Vec3f normal;
    Material* material;
};

class Surface
{
public:
    virtual bool Hit(const Ray& ray, float min, float max, SurfaceHitResult& result) const = 0;
    virtual bool ShadowHit(const Ray &r, float tmin, float tmax) const
    {
        SurfaceHitResult rec;
        return Hit(r, tmin, tmax, rec);
    }
    
protected:
    Material* mMaterial;
};