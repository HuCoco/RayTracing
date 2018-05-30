#pragma once
#include <Math/Accel/MathAccel.h>
#include <Math/Mat.h>
#include <Math/Vec4f.h>
template<>
class Mat<float, 4, 4>
{
public:
    inline Mat(const SIMD4X4F& m)
    {
        SIMD4F_USTORE4(m.x, m_Row0.Data());
        SIMD4F_USTORE4(m.y, m_Row1.Data());
        SIMD4F_USTORE4(m.z, m_Row2.Data());
        SIMD4F_USTORE4(m.w, m_Row3.Data());
    }

    inline Mat()
    {
        std::memset(m_Data, 0, sizeof(float) * 4 * 4);
    }

    inline Mat(const Mat<float, 4, 4>& m)
        : m_00(m.m_00)
        , m_01(m.m_01)
        , m_02(m.m_02)
        , m_03(m.m_03)
        , m_10(m.m_10)
        , m_11(m.m_11)
        , m_12(m.m_12)
        , m_13(m.m_13)
        , m_20(m.m_20)
        , m_21(m.m_21)
        , m_22(m.m_22)
        , m_23(m.m_23)
        , m_30(m.m_30)
        , m_31(m.m_31)
        , m_32(m.m_32)
        , m_33(m.m_33)
    {

    }

    inline Mat(
        float x00, float x01, float x02, float x03,
        float x10, float x11, float x12, float x13,
        float x20, float x21, float x22, float x23,
        float x30, float x31, float x32, float x33)
        : m_00(x00)
        , m_01(x01)
        , m_02(x02)
        , m_03(x03)
        , m_10(x10)
        , m_11(x11)
        , m_12(x12)
        , m_13(x13)
        , m_20(x20)
        , m_21(x21)
        , m_22(x22)
        , m_23(x23)
        , m_30(x30)
        , m_31(x31)
        , m_32(x32)
        , m_33(x33)
    {

    }

    inline Mat(const Vec4f& vx, const Vec4f& vy, const Vec4f& vz, const Vec4f& vw)
        : m_00(vx.x)
        , m_01(vx.y)
        , m_02(vx.z)
        , m_03(vx.w)
        , m_10(vy.x)
        , m_11(vy.y)
        , m_12(vy.z)
        , m_13(vy.w)
        , m_20(vz.x)
        , m_21(vz.y)
        , m_22(vz.z)
        , m_23(vz.w)
        , m_30(vw.x)
        , m_31(vw.y)
        , m_32(vw.z)
        , m_33(vw.w)
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
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        ASSERT_MSG(j >= 0 && j < 4, "over range.");
        return m_Data[i][j];
    }

    inline float& operator()(uint32_t i, uint32_t j) const
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        ASSERT_MSG(j >= 0 && j < 4, "over range.");
        return const_cast<float&>(m_Data[i][j]);
    }

    inline Vec4f GetRow(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        return Vec4f(m_Data[i][0], m_Data[i][1], m_Data[i][2], m_Data[i][3]);
    }

    inline void SetRow(uint32_t i, const Vec4f& v)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        m_Data[i][0] = v.x;
        m_Data[i][1] = v.y;
        m_Data[i][2] = v.z;
        m_Data[i][3] = v.w;
    }

    inline void SetRow(uint32_t i, float vx, float vy, float vz, float vw)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        m_Data[i][0] = vx;
        m_Data[i][1] = vy;
        m_Data[i][2] = vz;
        m_Data[i][3] = vw;
    }

    inline Vec4f GetColumn(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        return Vec4f(m_Data[0][i], m_Data[1][i], m_Data[2][i], m_Data[3][i]);
    }

    inline void SetColumn(uint32_t i, const Vec4f& v)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        m_Data[0][i] = v.x;
        m_Data[1][i] = v.y;
        m_Data[2][i] = v.z;
        m_Data[3][i] = v.w;
    }

    inline void SetColumn(uint32_t i, float vx, float vy, float vz, float vw)
    {
        ASSERT_MSG(i >= 0 && i < 4, "over range.");
        m_Data[0][i] = vx;
        m_Data[1][i] = vy;
        m_Data[2][i] = vz;
        m_Data[3][i] = vw;
    }

    inline Mat<float, 4, 4>& operator*=(float s)
    {
        m_Row0 = SIMD4F_MUL(SIMD4F_ULOAD4(m_Row0.Data()), SIMD4F_SET(s));
        m_Row1 = SIMD4F_MUL(SIMD4F_ULOAD4(m_Row1.Data()), SIMD4F_SET(s));
        m_Row2 = SIMD4F_MUL(SIMD4F_ULOAD4(m_Row2.Data()), SIMD4F_SET(s));
        m_Row3 = SIMD4F_MUL(SIMD4F_ULOAD4(m_Row3.Data()), SIMD4F_SET(s));
    }

    inline Mat<float, 4, 4>& operator/=(float s)
    {
        float rcp_s = 1.0f / s;
        m_Row0 = SIMD4F_DIV(SIMD4F_ULOAD4(m_Row0.Data()), SIMD4F_SET(rcp_s));
        m_Row1 = SIMD4F_DIV(SIMD4F_ULOAD4(m_Row1.Data()), SIMD4F_SET(rcp_s));
        m_Row2 = SIMD4F_DIV(SIMD4F_ULOAD4(m_Row2.Data()), SIMD4F_SET(rcp_s));
        m_Row3 = SIMD4F_DIV(SIMD4F_ULOAD4(m_Row3.Data()), SIMD4F_SET(rcp_s));
    }

    inline Mat<float, 4, 4> operator*(float s) const
    {
        return Mat<float, 4, 4>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_ULOAD3(m_Row3.Data())),
            SIMD4X4F_CREATE(SIMD4F_SET(s), SIMD4F_SET(s), SIMD4F_SET(s), SIMD4F_SET(s))));
    }

    inline Mat<float, 4, 4> operator/(float s) const
    {
        float rcp_s = 1.0f / s;
        return Mat<float, 4, 4>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_ULOAD3(m_Row3.Data())),
            SIMD4X4F_CREATE(SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s), SIMD4F_SET(rcp_s))));
    }

    inline Mat<float, 4, 4> operator*(const Mat<float, 4, 4>& m) const
    {
        return Mat<float, 4, 4>(SIMD4X4F_MUL(
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m_Row0.Data()), SIMD4F_ULOAD3(m_Row1.Data()), SIMD4F_ULOAD3(m_Row2.Data()), SIMD4F_ULOAD3(m_Row3.Data())),
            SIMD4X4F_CREATE(SIMD4F_ULOAD3(m.m_Row0.Data()), SIMD4F_ULOAD3(m.m_Row1.Data()), SIMD4F_ULOAD3(m.m_Row2.Data()), SIMD4F_ULOAD3(m_Row3.Data()))));
    }

protected:
    union
    {
        float m_Data[4][4];
        struct
        {
            Vec4f m_Row0;
            Vec4f m_Row1;
            Vec4f m_Row2;
            Vec4f m_Row3;
        };
        struct
        {
            float m_00, m_01, m_02, m_03;
            float m_10, m_11, m_12, m_13;
            float m_20, m_21, m_22, m_23;
            float m_30, m_31, m_32, m_33;
        };
    };
};