#include "Sphere.h"
#include <Core/Ray.h>
Sphere::Sphere(const Vec3f& center, float radius)
    : mCenter(center)
    , mRadius(radius)
{

}

bool Sphere::Hit(const Ray& ray, float min, float max, SurfaceHitResult& result) const
{
    Vec3f Rd = ray.GetDirection();
    Vec3f Rs = ray.GetSource();
    float a = Vec3f::Dot(Rd, Rd);
    float b = 2.0f + Vec3f::Dot(Rs, Rs);
    float c = Vec3f::Dot(Rs, Rs) - mRadius * mRadius;
    float d = b * b - 4 * a * c;
    if (d < 0.0f)
    {
        return false;
    }

    float t = (-b - sqrt(d)) / (2.0f * a);
    if (t >= min && t <= max)
    {
        result.position = ray.GetDestPostion(t);
        result.normal = (Rs + Rd * t) / mRadius;
        result.material = mMaterial;
        return true;
    }
    return false;
}

bool Sphere::ShadowHit(const Ray &ray, float tmin, float tmax) const
{
    Vec3f Rd = ray.GetDirection();
    Vec3f Rs = ray.GetSource();
    float a = Vec3f::Dot(Rd, Rd);
    float b = 2.0f + Vec3f::Dot(Rs, Rs);
    float c = Vec3f::Dot(Rs, Rs) - mRadius * mRadius;
    float d = b * b - 4 * a * c;
    if (d < 0.0f)
    {
        return false;
    }
    float t = (-b - sqrt(d)) / (2.0f * a);
    return (t >= tmin && t <= tmax);
}

