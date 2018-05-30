#pragma once
#include <Math/Accel/MathAccel.h>
#include <Math/Mat.h>
#include <Math/Vec3f.h>

typedef Mat<float, 3, 3> Mat3x3f;

template<>
class Mat<float, 3 ,3>
{
public:
    inline Mat(const SIMD4X4F& m)
    {
       SIMD4F_USTORE3(m.x, m_Row0.Data());
       SIMD4F_USTORE3(m.y, m_Row1.Data());
       SIMD4F_USTORE3(m.z, m_Row2.Data());
    }

    inline Mat()
    {
        std::memset(m_Data, 0, sizeof(float) * 3 * 3);
    }

    inline Mat(const Mat<float, 3, 3>& m)
        : m_00(m.m_00)
        , m_01(m.m_01)
        , m_02(m.m_02)
        , m_10(m.m_10)
        , m_11(m.m_11)
        , m_12(m.m_12)
        , m_20(m.m_20)
        , m_21(m.m_21)
        , m_22(m.m_22)
    {

    }

    inline Mat(float x00, float x01, float x02, float x10, float x11, float x12, float x20, float x21, float x22)
        : m_00(x00)
        , m_01(x01)
        , m_02(x02)
        , m_10(x10)
        , m_11(x11)
        , m_12(x12)
        , m_20(x20)
        , m_21(x21)
        , m_22(x22)
    {

    }

    inline Mat(const Vec3f& vx, const Vec3f& vy, const Vec3f& vz)
        : m_00(vx.x)
        , m_01(vx.y)
        , m_02(vx.z)
        , m_10(vy.x)
        , m_11(vy.y)
        , m_12(vy.z)
        , m_20(vz.x)
        , m_21(vz.y)
        , m_22(vz.z)

    {

    }

    inline float* Data()
    {
        return &m_00;
    }

    inline const float* Data() const
    {
        return &m_00;
    }
    
    inline float operator()(uint32_t i, uint32_t j)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        ASSERT_MSG(j >= 0 && j < 3, "over range.");
        return m_Data[i][j];
    }

    inline float& operator()(uint32_t i, uint32_t j) const 
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        ASSERT_MSG(j >= 0 && j < 3, "over range.");
        return const_cast<float&>(m_Data[i][j]);
    }

    inline Vec3f GetRow(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        return Vec3f(m_Data[i][0], m_Data[i][1], m_Data[i][2]);
    }

    inline void SetRow(uint32_t i,const Vec3f& v)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        m_Data[i][0] = v.x;
        m_Data[i][1] = v.y;
        m_Data[i][2] = v.z;
    }

    inline void SetRow(uint32_t i, float vx, float vy, float vz)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        m_Data[i][0] = vx;
        m_Data[i][1] = vy;
        m_Data[i][2] = vz;
    }

    inline Vec3f GetColumn(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        return Vec3f(m_Data[0][i], m_Data[1][i], m_Data[2][i]);
    }

    inline void SetColumn(uint32_t i, const Vec3f& v)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        m_Data[0][i] = v.x;
        m_Data[1][i] = v.y;
        m_Data[2][i] = v.z;
    }

    inline void SetColumn(uint32_t i, float vx, float vy, float vz)
    {
        ASSERT_MSG(i >= 0 && i < 3, "over range.");
        m_Data[0][i] = vx;
        m_Data[1][i] = vy;
        m_Data[2][i] = vz;
    }

    inline Mat<float, 3, 3>& operator*=(float s)
    {
        m_Row0 = SIMD4F_MUL(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_SET(s));
        m_Row1 = SIMD4F_MUL(SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_SET(s));
        m_Row2 = SIMD4F_MUL(SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_SET(s));
    }

    inline Mat<float, 3, 3>& operator/=(float s)
    {
        float rcp_s = 1.0f / s;
        m_Row0 = SIMD4F_DIV(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_SET(rcp_s));
        m_Row1 = SIMD4F_DIV(SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_SET(rcp_s));
        m_Row2 = SIMD4F_DIV(SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_SET(rcp_s));
    }

    inline void Transpose()
    {
        *this = SIMD4X4F_TRANSPOSE(
            SIMD4X4F_CREATE(
                SIMD4F_ULOAD3(m_Row0.Data()),
                SIMD4F_ULOAD3(m_Row1.Data()),
                SIMD4F_ULOAD3(m_Row2.Data()),
                SIMD4F_CREATE(0.0f,0.0f,0.0f,1.0f)
            )
        );
    }

    inline Mat<float, 3, 3> GetTransposed()
    {
        return Mat<float, 3, 3>(SIMD4X4F_TRANSPOSE(
            SIMD4X4F_CREATE(
                SIMD4F_ULOAD3(m_Row0.Data()),
                SIMD4F_ULOAD3(m_Row1.Data()),
                SIMD4F_ULOAD3(m_Row2.Data()),
                SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f))));
    }

    inline void Invert()
    {
        
    }

    inline Mat<float, 3, 3> GetInverted()
    {
        return Mat<float, 3, 3>(SIMD4X4F_INVERSE(
            SIMD4X4F_CREATE(
                SIMD4F_ULOAD3(m_Row0.Data()),
                SIMD4F_ULOAD3(m_Row1.Data()),
                SIMD4F_ULOAD3(m_Row2.Data()),
                SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f))));
    }

    inline Mat<float, 3, 3> operator*(float s) const
    {
        return Mat<float, 3, 3>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f)),
            SIMD4X4F_CREATE(SIMD4F_SET(s), SIMD4F_SET(s), SIMD4F_SET(s), SIMD4F_SET(s))));
    }

    inline Mat<float, 3, 3> operator/(float s) const
    {
        float rcp_s = 1.0f / s;
        return Mat<float, 3, 3>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f)),
            SIMD4X4F_CREATE(SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s))));
    }

    inline Mat<float,3,3> operator*(const Mat<float, 3, 3>& m) const
    {
        return Mat<float, 3, 3>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f)),
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m.m_Row0.Data()), SIMD4F_ULOAD3(m.m_Row1.Data()), SIMD4F_ULOAD3(m.m_Row2.Data()), SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f))));
    }



protected:
    union 
    {
        float m_Data[3][3];
        struct 
        {
            Vec3f m_Row0;
            Vec3f m_Row1;
            Vec3f m_Row2;
        };
        struct 
        {
            float m_00, m_01, m_02;
            float m_10, m_11, m_12;
            float m_20, m_21, m_22;
        };
    };
   
};