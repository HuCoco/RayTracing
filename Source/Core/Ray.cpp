#include "Ray.h"

Ray::Ray(const Vec3f& source, const Vec3f& direction)
    : mSource(source)
    , mDirection(direction)
{

}

Vec3f Ray::GetDestPostion(float t) const
{
    return mSource + mDirection * t;
}

