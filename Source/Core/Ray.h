#pragma once
#include <Math/Vec3f.h>

class Ray
{
    Ray() = default;
    Ray(const Vec3f& source, const Vec3f& direction);

    inline void SetSource(const Vec3f& source) { mSource = source; }
    inline void SetDirection(const Vec3f& direction) { mDirection = direction; }
    inline Vec3f GetSource() { return mSource; }
    inline Vec3f GetDirection() { return mDirection; }
private:
    Vec3f mSource{ 0.0f,0.0f,0.0f };
    Vec3f mDirection{ 0.0f,0.0f,0.0f };
};