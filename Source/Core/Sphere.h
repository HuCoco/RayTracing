#pragma once
#include <Core/Surface.h>

class Sphere : public Surface
{
public:
    Sphere(const Vec3f& center, float radius);
    virtual bool Hit(const Ray& ray, float min, float max, SurfaceHitResult& result) const override;
    virtual bool ShadowHit(const Ray &r, float tmin, float tmax) const override;
private:
    Vec3f mCenter;
    float mRadius;
};
