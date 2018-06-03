#pragma once
#include <Math/Vec3f.h>

class Ray
{
public:
    Ray() = default;
    Ray(const Vec3f& source, const Vec3f& direction);

    inline void SetSource(const Vec3f& source) { mSource = source; }
    inline void SetDirection(const Vec3f& direction) { mDirection = direction; }
    inline Vec3f GetSource() const { return mSource; }
    inline Vec3f GetDirection() const { return mDirection; }

    inline Vec3f GetDestPostion(float t) const;

private:
    Vec3f mSource{ 0.0f,0.0f,0.0f };
    Vec3f mDirection{ 0.0f,0.0f,0.0f };
};