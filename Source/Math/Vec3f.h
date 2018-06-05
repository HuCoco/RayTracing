#pragma once
#include <Math/Accel/MathAccel.h>
#include <Math/Vec.h>

using Vec3f = Vec<float, 3>;


template <>
class Vec<float, 3>
{
public:
    inline Vec(const SIMD4F& v)
    {
        SIMD4F_USTORE3(v, this->Data());
    }

public:
    inline Vec()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {

    }

    inline Vec(const Vec<float, 3>& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
    {

    }

    inline Vec(float vx, float vy, float vz)
        : x(vx)
        , y(vy)
        , z(vz)
    {

    }

    inline Vec(float* v)
        : x(v[0])
        , y(v[1])
        , z(v[2])
    {

    }

    inline float* Data()
    {
        return &x;
    }

    inline const float* Data() const
    {
        return &x;
    }

    inline float& operator()(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator()(uint32_t i) const
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline float& operator[](uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator[](uint32_t i) const
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline Vec<float, 3> operator-() const
    {
        return Vec<float, 3>(SIMD4F_SUB(SIMD4F_ZERO(), SIMD4F_ULOAD3(this->Data())));
    }

    inline Vec<float, 3> operator*(const Vec<float, 3>& v) const
    {
        return Vec<float, 3>(SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data())));
    }

    inline Vec<float, 3> operator/(const Vec<float, 3>& v) const
    {
        return Vec<float, 3>(SIMD4F_DIV(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data())));
    }

    inline Vec<float, 3> operator+(const Vec<float, 3>& v) const
    {
        return Vec<float, 3>(SIMD4F_ADD(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data())));
    }

    inline Vec<float, 3> operator-(const Vec<float, 3>& v) const
    {
        return Vec<float, 3>(SIMD4F_SUB(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data())));
    }

    inline Vec<float, 3> operator*(float s) const
    {
        return Vec<float, 3>(SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 3> operator/(float s) const
    {
        return Vec<float, 3>(SIMD4F_DIV(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 3> operator+(float s) const
    {
        return Vec<float, 3>(SIMD4F_ADD(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 3> operator-(float s) const
    {
        return Vec<float, 3>(SIMD4F_SUB(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 3> operator*=(const Vec<float, 3>& v)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data()));
        return *this;
    }

    inline Vec<float, 3> operator/=(const Vec<float, 3>& v)
    {
        *this = SIMD4F_DIV(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data()));
        return *this;
    }

    inline Vec<float, 3> operator+=(const Vec<float, 3>& v)
    {
        *this = SIMD4F_ADD(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data()));
        return *this;
    }

    inline Vec<float, 3> operator-=(const Vec<float, 3>& v)
    {
        *this = SIMD4F_SUB(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(v.Data()));
        return *this;
    }

    inline Vec<float, 3> operator*=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 3> operator/=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 3> operator+=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 3> operator-=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD3(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline bool operator==(const Vec<float, 3>& v) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if ((*this)[i] != v[i])
            {
                return false;
            }
        }
        return true;
    }

    inline bool operator!=(const Vec<float, 3>& v) const
    {
        return !operator==(v);
    }

    inline float GetLengthSquared() const
    {
        return SIMD4F_DOT3_SCALAR(SIMD4F_ULOAD3(this->Data()), SIMD4F_ULOAD3(this->Data()));
    }

    inline float GetLength() const
    {
        return SIMD4F_GET_X(SIMD4F_LENGTH3(SIMD4F_ULOAD3(this->Data())));
    }

    inline Vec<float, 3> GetNormalized() const
    {
        return Vec<float, 3>(SIMD4F_NORMALIZE3(SIMD4F_ULOAD3(this->Data())));
    }

    inline void Normalized()
    {
        *this = SIMD4F_NORMALIZE3(SIMD4F_ULOAD3(this->Data()));
    }

    static inline Vec3f Cross(const Vec3f& lhs, const Vec3f& rhs)
    {
        return Vec3f(SIMD4F_CROSS3(SIMD4F_ULOAD3(lhs.Data()), SIMD4F_ULOAD3(rhs.Data())));
    }

    static inline float Dot(const Vec3f& lhs, const Vec3f& rhs)
    {
        return SIMD4F_DOT3_SCALAR(SIMD4F_ULOAD3(lhs.Data()), SIMD4F_ULOAD3(rhs.Data()));
    }

    friend Vec3f operator* (float s, const Vec3f &v);

public:
    float x;
    float y;
    float z;
};

inline Vec3f operator* (float s, const Vec3f &v)
{
    return Vec<float, 3>(SIMD4F_MUL(SIMD4F_ULOAD3(v.Data()), SIMD4F_SET(s)));
}