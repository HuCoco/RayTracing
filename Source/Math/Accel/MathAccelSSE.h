#pragma once
#include <Platform/Platform.h>
#include <limits>
#include <xmmintrin.h>
#include <smmintrin.h>  //SSE 4.1
#define USE_SSE4_1

#include <memory>
#include <immintrin.h>

typedef __m128 SIMD4F;

GE_ALIGN_HEAD(16) struct SIMD4X4F
{
    SIMD4F x, y, z, w;
} GE_ALIGN_TAIL(16);

union SIMD4F_UNION
{
    SIMD4F s;
    float f[4];
};

static inline SIMD4F SIMD4F_CREATE(float x, float y, float z = 0.0f, float w = 0.0f)
{
    return _mm_setr_ps(x, y, z, w);
}

static inline SIMD4F SIMD4F_ZERO()
{
    return _mm_setzero_ps();
}

static inline SIMD4F SIMD4F_ULOAD2(const float* arr)
{
    return SIMD4F_CREATE(arr[0], arr[1]);
}

static inline SIMD4F SIMD4F_ULOAD3(const float* arr)
{
    return SIMD4F_CREATE(arr[0], arr[1], arr[2]);
}

static inline SIMD4F SIMD4F_ULOAD4(const float* arr)
{
    return _mm_loadu_ps(arr);
}

static inline void SIMD4F_USTORE2(const SIMD4F& val, float* arr)
{
    std::memcpy(arr, &val, sizeof(float) * 2);
}

static inline void SIMD4F_USTORE3(const SIMD4F& val, float* arr)
{
    std::memcpy(arr, &val, sizeof(float) * 3);
}

static inline void SIMD4F_USTORE4(const SIMD4F& val, float* arr)
{
    _mm_storeu_ps(arr, val);
}

static inline SIMD4F SIMD4F_SET(float v)
{
    return  _mm_set1_ps(v);
}

static inline SIMD4F SIMD4F_SET_X(const SIMD4F& v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
}

static inline SIMD4F SIMD4F_SET_Y(const SIMD4F& v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
}

static inline SIMD4F SIMD4F_SET_Z(const SIMD4F& v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
}

static inline SIMD4F SIMD4F_SET_W(const SIMD4F& v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
}


static inline SIMD4F SIMD4F_ADD(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return _mm_add_ps(lhs, rhs);
}

static inline SIMD4F SIMD4F_SUB(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return _mm_sub_ps(lhs, rhs);
}

static inline SIMD4F SIMD4F_MUL(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return _mm_mul_ps(lhs, rhs);
}

static inline SIMD4F SIMD4F_DIV(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return _mm_div_ps(lhs, rhs);
}

static inline SIMD4F SIMD4F_SUM(const SIMD4F& v)
{
    const SIMD4F s1 = SIMD4F_ADD(SIMD4F_SET_X(v), SIMD4F_SET_Y(v));
    const SIMD4F s2 = SIMD4F_ADD(s1, SIMD4F_SET_Z(v));
    const SIMD4F s3 = SIMD4F_ADD(s2, SIMD4F_SET_W(v));
    return s3;
}

static inline SIMD4F SIMD4F_MUL_ADD(const SIMD4F& m1, const SIMD4F& m2, const SIMD4F& a)
{
    return SIMD4F_ADD(SIMD4F_MUL(m1, m2), a);
}

static inline SIMD4F SIMD4F_RECIPROCAL(const SIMD4F& v)
{
    SIMD4F s = _mm_rcp_ps(v);
    const SIMD4F two = SIMD4F_SET(2.0f);
    return SIMD4F_MUL(s, SIMD4F_SUB(two, SIMD4F_MUL(v, s)));
}

static inline SIMD4F SIMD4F_SQRT(const SIMD4F& v)
{
    return _mm_sqrt_ps(v);
}

static inline SIMD4F SIMD4F_RSQRT(const SIMD4F& v)
{
    SIMD4F s = _mm_rsqrt_ps(v);
    const SIMD4F half = SIMD4F_SET(0.5f);
    const SIMD4F three = SIMD4F_SET(3.0f);
    return SIMD4F_MUL(SIMD4F_MUL(s, half), SIMD4F_SUB(three, SIMD4F_MUL(s, SIMD4F_MUL(v, s))));
}

static inline float SIMD4F_GET_X(const SIMD4F& s)
{
    SIMD4F_UNION u = { s };
    return u.f[0];
}

static inline float SIMD4F_GET_Y(const SIMD4F& s)
{
    SIMD4F_UNION u = { s };
    return u.f[1];
}

static inline float SIMD4F_GET_Z(const SIMD4F& s)
{
    SIMD4F_UNION u = { s };
    return u.f[2];
}

static inline float SIMD4F_GET_W(const SIMD4F& s)
{
    SIMD4F_UNION u = { s };
    return u.f[3];
}

static inline SIMD4F SIMD4F_DOT2(const SIMD4F& lhs, const SIMD4F& rhs)
{
    const SIMD4F m = SIMD4F_MUL(lhs, rhs);
    const SIMD4F s1 = SIMD4F_ADD(SIMD4F_SET_X(m), SIMD4F_SET_Y(m));
    return s1;
}

static inline float SIMD4F_DOT2_SCALAR(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return SIMD4F_GET_X(SIMD4F_DOT2(lhs, rhs));
}

static inline SIMD4F SIMD4F_DOT3(const SIMD4F& lhs, const SIMD4F& rhs)
{
#if defined(USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0x7f);
#else
    const SIMD4F mask = SIMD4F_CREATE(0xffffffff, 0xffffffff, 0xffffffff, 0x0);
    const SIMD4F m = _mm_mul_ps(lhs, rhs);
    const SIMD4F s0 = _mm_and_ps(m, mask);
    const SIMD4F s1 = _mm_add_ps(s0, _mm_movehl_ps(s0, s0));
    const SIMD4F s2 = _mm_add_ss(s1, _mm_shuffle_ps(s1, s1, 1));
    return _mm_shuffle_ps(s2, s2, 0);
#endif
}

static inline float SIMD4F_DOT3_SCALAR(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return SIMD4F_GET_X(SIMD4F_DOT3(lhs, rhs));
}

static inline SIMD4F SIMD4F_DOT4(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return SIMD4F_SUM(SIMD4F_MUL(lhs, rhs));
}

static inline float SIMD4F_DOT4_SCALAR(const SIMD4F& lhs, const SIMD4F& rhs)
{
    return SIMD4F_GET_X(SIMD4F_DOT4(lhs, rhs));
}

static inline SIMD4F SIMD4F_CROSS3(const SIMD4F& lhs, const SIMD4F& rhs)
{
    const SIMD4F lyzx = _mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(3, 0, 2, 1));
    const SIMD4F lzxy = _mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(3, 1, 0, 2));

    const SIMD4F ryzx = _mm_shuffle_ps(rhs, rhs, _MM_SHUFFLE(3, 0, 2, 1));
    const SIMD4F rzxy = _mm_shuffle_ps(rhs, rhs, _MM_SHUFFLE(3, 1, 0, 2));

    return _mm_sub_ps(_mm_mul_ps(lyzx, rzxy), _mm_mul_ps(lzxy, ryzx));
}

static inline SIMD4F SIMD4F_LENGTH4(const SIMD4F& v)
{
    return SIMD4F_SQRT(SIMD4F_DOT4(v, v));
}

static inline SIMD4F SIMD4F_LENGTH3(const SIMD4F& v)
{
    return SIMD4F_SQRT(SIMD4F_DOT3(v, v));
}

static inline SIMD4F SIMD4F_LENGTH2(const SIMD4F& v)
{
    return SIMD4F_SQRT(SIMD4F_DOT2(v, v));
}

static inline SIMD4F SIMD4F_LENGTH4_SQUARED(const SIMD4F& v)
{
    return SIMD4F_DOT4(v, v);
}

static inline SIMD4F SIMD4F_LENGTH3_SQUARED(const SIMD4F& v)
{
    return SIMD4F_DOT4(v, v);
}

static inline float SIMD4F_LENGTH3_SQUARED_SCALAR(const SIMD4F& v)
{
    return SIMD4F_DOT3_SCALAR(v, v);
}

static inline SIMD4F SIMD4F_SHUFFLE_WXYZ(const SIMD4F& s)
{
    return _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 1, 0, 3));
}

static inline SIMD4F SIMD4F_NORMALIZE4(const SIMD4F& a)
{
    SIMD4F invlen = SIMD4F_RSQRT(SIMD4F_DOT4(a, a));
    return SIMD4F_MUL(a, invlen);
}

static inline SIMD4F SIMD4F_NORMALIZE3(const SIMD4F& a)
{
    SIMD4F invlen = SIMD4F_RSQRT(SIMD4F_DOT3(a, a));
    return SIMD4F_MUL(a, invlen);
}

static inline SIMD4F SIMD4F_NORMALIZE2(const SIMD4F& a)
{
    SIMD4F invlen = SIMD4F_RSQRT(SIMD4F_DOT2(a, a));
    return SIMD4F_MUL(a, invlen);
}

static inline SIMD4F SIMD4F_LENGTH2_SQUARED(const SIMD4F& v)
{
    return SIMD4F_DOT2(v, v);
}

static inline SIMD4F SIMD4F_SHUFFLE_ZWXY(const SIMD4F& s)
{
    return _mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 0, 3, 2));
}

static inline SIMD4F SIMD4F_SHUFFLE_YZWX(const SIMD4F& s)
{
    return _mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 3, 2, 1));
}

static inline SIMD4F SIMD4F_MERGE_HIGH(const SIMD4F& xyzw, const SIMD4F& abcd)
{
    return _mm_movehl_ps(abcd, xyzw);
}

static inline SIMD4F SIMD4F_FLIP_SIGN_0101(const SIMD4F& s)
{
    return _mm_xor_ps(s, SIMD4F_CREATE(0x00000000, 0x80000000, 0x00000000, 0x80000000));
}

static inline SIMD4F SIMD4F_FLIP_SIGN_1010(const SIMD4F& s)
{
    return _mm_xor_ps(s, SIMD4F_CREATE(0x80000000, 0x00000000, 0x80000000, 0x00000000));
}

static inline SIMD4F SIMD4F_MIN(const SIMD4F& a, const SIMD4F& b)
{
    return _mm_min_ps(a, b);
}

static inline SIMD4F SIMD4F_MAX(const SIMD4F& a, const SIMD4F& b)
{
    return _mm_max_ps(a, b);
}

static inline SIMD4X4F SIMD4X4F_CREATE(const SIMD4F& x, const SIMD4F& y, const SIMD4F& z, const SIMD4F& w)
{
    SIMD4X4F s = { x, y, z, w };
    return s;
}

static inline SIMD4X4F SIMD4X4F_IDENTITY()
{
    return SIMD4X4F_CREATE(
        SIMD4F_CREATE(1.0f, 0.0f, 0.0f, 0.0f),
        SIMD4F_CREATE(0.0f, 1.0f, 0.0f, 0.0f),
        SIMD4F_CREATE(0.0f, 0.0f, 1.0f, 0.0f),
        SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 1.0f));
}

static inline void SIMD4X4F_ULOAD(SIMD4X4F& m, const float *f)
{
    m.x = SIMD4F_ULOAD4(f + 0);
    m.y = SIMD4F_ULOAD4(f + 4);
    m.z = SIMD4F_ULOAD4(f + 8);
    m.w = SIMD4F_ULOAD4(f + 12);
}

static inline void SIMD4X4F_TRANSPOSE_INPLACE(SIMD4X4F& s)
{
    _MM_TRANSPOSE4_PS(s.x, s.y, s.z, s.w);
}

static inline SIMD4X4F SIMD4X4F_TRANSPOSE(const SIMD4X4F& s)
{
    SIMD4X4F out;
    out = s;
    SIMD4X4F_TRANSPOSE_INPLACE(out);
    return out;
}

static inline SIMD4F SIMD4X4F_SUM(const SIMD4X4F& a)
{
    SIMD4F result = SIMD4F_ZERO();
    result = SIMD4F_ADD(a.x, a.y);
    result = SIMD4F_ADD(result, a.z);
    result = SIMD4F_ADD(result, a.w);
    return result;
}

static inline SIMD4F SIMD4X4F_MATRIX_VECTOR_MUL(const SIMD4X4F& a, const SIMD4F& b)
{

    const SIMD4F x = a.x;
    const SIMD4F y = a.y;
    const SIMD4F z = a.z;
    const SIMD4F w = a.w;
    const SIMD4F v = b;
    const SIMD4F vx = SIMD4F_SET_X(v);
    const SIMD4F vy = SIMD4F_SET_Y(v);
    const SIMD4F vz = SIMD4F_SET_Z(v);
    const SIMD4F vw = SIMD4F_SET_W(v);

    SIMD4F result = SIMD4F_ZERO();
    result = SIMD4F_ADD(
        SIMD4F_MUL(x, vx), SIMD4F_ADD(
            SIMD4F_MUL(y, vy), SIMD4F_ADD(
                SIMD4F_MUL(z, vz), SIMD4F_MUL(w, vw))));
    return result;
}

static inline SIMD4F SIMD4X4F_MATRIX_VECTOR3_MUL(const SIMD4X4F& a, const SIMD4F& b)
{
    return SIMD4F_ADD(
        SIMD4F_MUL(a.x, SIMD4F_SET_X(b)), SIMD4F_ADD(
            SIMD4F_MUL(a.y, SIMD4F_SET_Y(b)), SIMD4F_MUL(a.z, SIMD4F_SET_Z(b))));
}

static inline SIMD4F SIMD4X4F_MATRIX_POINT3_MUL(const SIMD4X4F& a, const SIMD4F& b)
{
    return SIMD4F_ADD(
        SIMD4F_MUL(a.x, SIMD4F_SET_X(b)), SIMD4F_ADD(
            SIMD4F_MUL(a.y, SIMD4F_SET_Y(b)), SIMD4F_ADD(
                SIMD4F_MUL(a.z, SIMD4F_SET_Z(b)), a.w)));
}

static inline SIMD4F SIMD4X4F_INV_ORTHO_MATRIX_POINT3_MUL(const SIMD4X4F& a, const SIMD4F& b)
{
    SIMD4F translation = SIMD4F_SUB(b, a.w);

    SIMD4X4F transpose = a;

    transpose.w = SIMD4F_CREATE(0, 0, 0, 0);
    SIMD4X4F_TRANSPOSE_INPLACE(transpose);

    return SIMD4X4F_MATRIX_POINT3_MUL(transpose, translation);
}


static inline SIMD4F SIMD4X4F_INV_ORTHO_MATRIX_VECTOR3_MUL(const SIMD4X4F& a, const SIMD4F& b)
{
    SIMD4F translation = b;

    SIMD4X4F transpose = a;

    transpose.w = SIMD4F_CREATE(0.0f, 0.0f, 0.0f, 0.0f);
    SIMD4X4F_TRANSPOSE_INPLACE(transpose);

    return SIMD4X4F_MATRIX_VECTOR3_MUL(transpose, translation);
}

static inline SIMD4X4F SIMD4X4F_MATRIX_MUL(const SIMD4X4F& a, const SIMD4X4F& b)
{
    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4X4F_MATRIX_VECTOR_MUL(a, b.x);
    result.y = SIMD4X4F_MATRIX_VECTOR_MUL(a, b.y);
    result.z = SIMD4X4F_MATRIX_VECTOR_MUL(a, b.z);
    result.w = SIMD4X4F_MATRIX_VECTOR_MUL(a, b.w);
    return result;
}

static inline SIMD4X4F SIMD4X4F_PERSPECTIVE(float fovy_radians, float aspect, float znear, float zfar)
{

    float deltaz = zfar - znear;
    float cotangent = tanf(3.1415926f / 2.0f - fovy_radians * 0.5f);

    float a = cotangent / aspect;
    float b = cotangent;
    float c = -(zfar + znear) / deltaz;
    float d = -2 * znear * zfar / deltaz;

    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_CREATE(a, 0, 0, 0);
    result.y = SIMD4F_CREATE(0, b, 0, 0);
    result.z = SIMD4F_CREATE(0, 0, c, -1);
    result.w = SIMD4F_CREATE(0, 0, d, 0);
    return result;
}

static inline SIMD4X4F SIMD4X4F_ORTHO(float left, float right, float bottom, float top, float znear, float zfar)
{

    float deltax = right - left;
    float deltay = top - bottom;
    float deltaz = zfar - znear;

    float a = 2.0f / deltax;
    float b = -(right + left) / deltax;
    float c = 2.0f / deltay;
    float d = -(top + bottom) / deltay;
    float e = -2.0f / deltaz;
    float f = -(zfar + znear) / deltaz;

    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_CREATE(a, 0, 0, 0);
    result.y = SIMD4F_CREATE(0, c, 0, 0);
    result.z = SIMD4F_CREATE(0, 0, e, 0);
    result.w = SIMD4F_CREATE(b, d, f, 1);
    return result;
}

static inline SIMD4X4F SIMD4X4F_LOOKAT(SIMD4F eye, SIMD4F center, SIMD4F up)
{
    SIMD4F zaxis = SIMD4F_NORMALIZE3(SIMD4F_SUB(center, eye));
    SIMD4F xaxis = SIMD4F_NORMALIZE3(SIMD4F_CROSS3(zaxis, up));
    SIMD4F yaxis = SIMD4F_CROSS3(xaxis, zaxis);

    zaxis = SIMD4F_SUB(SIMD4F_ZERO(), zaxis);

    float x = -SIMD4F_DOT3_SCALAR(xaxis, eye);
    float y = -SIMD4F_DOT3_SCALAR(yaxis, eye);
    float z = -SIMD4F_DOT3_SCALAR(zaxis, eye);

    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = xaxis;
    result.y = yaxis;
    result.z = zaxis;

    result.w = SIMD4F_CREATE(0, 0, 0, 1);
    SIMD4X4F_TRANSPOSE_INPLACE(result);
    result.w = SIMD4F_CREATE(x, y, z, 1);
    return result;

}

static inline SIMD4X4F SIMD4X4F_TRANSLATION(float x, float y, float z) {
    return SIMD4X4F_CREATE(
        SIMD4F_CREATE(1.0f, 0.0f, 0.0f, 0.0f),
        SIMD4F_CREATE(0.0f, 1.0f, 0.0f, 0.0f),
        SIMD4F_CREATE(0.0f, 0.0f, 1.0f, 0.0f),
        SIMD4F_CREATE(x, y, z, 1.0f));
}

static inline SIMD4X4F SIMD4X4F_AXIS_ROTATION(float radians, SIMD4F axis) {

    radians = -radians;

    axis = SIMD4F_NORMALIZE3(axis);

    const float sine = sinf(radians);
    const float cosine = cosf(radians);

    const float x = SIMD4F_GET_X(axis);
    const float y = SIMD4F_GET_Y(axis);
    const float z = SIMD4F_GET_Z(axis);

    const float ab = x * y * (1 - cosine);
    const float bc = y * z * (1 - cosine);
    const float ca = z * x * (1 - cosine);

    const float tx = x * x;
    const float ty = y * y;
    const float tz = z * z;

    const SIMD4F i = SIMD4F_CREATE(tx + cosine * (1 - tx), ab - z * sine, ca + y * sine, 0);
    const SIMD4F j = SIMD4F_CREATE(ab + z * sine, ty + cosine * (1 - ty), bc - x * sine, 0);
    const SIMD4F k = SIMD4F_CREATE(ca - y * sine, bc + x * sine, tz + cosine * (1 - tz), 0);

    return SIMD4X4F_CREATE(i, j, k, SIMD4F_CREATE(0, 0, 0, 1));

}

static inline SIMD4X4F SIMD4X4F_ADD(const SIMD4X4F& a, const SIMD4X4F& b)
{
    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_ADD(a.x, b.x);
    result.y = SIMD4F_ADD(a.y, b.y);
    result.z = SIMD4F_ADD(a.z, b.z);
    result.w = SIMD4F_ADD(a.w, b.w);
    return result;
}

static inline SIMD4X4F SIMD4X4F_SUB(const SIMD4X4F& a, const SIMD4X4F& b)
{
    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_SUB(a.x, b.x);
    result.y = SIMD4F_SUB(a.y, b.y);
    result.z = SIMD4F_SUB(a.z, b.z);
    result.w = SIMD4F_SUB(a.w, b.w);
    return result;
}

static inline SIMD4X4F SIMD4X4F_MUL(const SIMD4X4F& a, const SIMD4X4F& b)
{
    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_MUL(a.x, b.x);
    result.y = SIMD4F_MUL(a.y, b.y);
    result.z = SIMD4F_MUL(a.z, b.z);
    result.w = SIMD4F_MUL(a.w, b.w);
    return result;
}

static inline SIMD4X4F SIMD4X4F_DIV(const SIMD4X4F& a, const SIMD4X4F& b)
{
    SIMD4X4F result = SIMD4X4F_IDENTITY();
    result.x = SIMD4F_DIV(a.x, b.x);
    result.y = SIMD4F_DIV(a.y, b.y);
    result.z = SIMD4F_DIV(a.z, b.z);
    result.w = SIMD4F_DIV(a.w, b.w);
    return result;
}

static inline SIMD4X4F SIMD4X4F_INVERSE(const SIMD4X4F& a)
{
    const SIMD4F c0 = a.x;
    const SIMD4F c1 = a.y;
    const SIMD4F c2 = a.z;
    const SIMD4F c3 = a.w;

    const SIMD4F c0_wxyz = SIMD4F_SHUFFLE_WXYZ(c0);
    const SIMD4F c0_zwxy = SIMD4F_SHUFFLE_ZWXY(c0);
    const SIMD4F c0_yzwx = SIMD4F_SHUFFLE_YZWX(c0);

    const SIMD4F c1_wxyz = SIMD4F_SHUFFLE_WXYZ(c1);
    const SIMD4F c1_zwxy = SIMD4F_SHUFFLE_ZWXY(c1);
    const SIMD4F c1_yzwx = SIMD4F_SHUFFLE_YZWX(c1);

    const SIMD4F c2_wxyz = SIMD4F_SHUFFLE_WXYZ(c2);
    const SIMD4F c2_zwxy = SIMD4F_SHUFFLE_ZWXY(c2);
    const SIMD4F c2_yzwx = SIMD4F_SHUFFLE_YZWX(c2);

    const SIMD4F c3_wxyz = SIMD4F_SHUFFLE_WXYZ(c3);
    const SIMD4F c3_zwxy = SIMD4F_SHUFFLE_ZWXY(c3);
    const SIMD4F c3_yzwx = SIMD4F_SHUFFLE_YZWX(c3);

    const SIMD4F c0_wxyz_x_c1 = SIMD4F_MUL(c0_wxyz, c1);
    const SIMD4F c0_wxyz_x_c1_yzwx = SIMD4F_MUL(c0_wxyz, c1_yzwx);
    const SIMD4F c0_wxyz_x_c1_zwxy = SIMD4F_MUL(c0_wxyz, c1_zwxy);

    const SIMD4F c2_wxyz_x_c3 = SIMD4F_MUL(c2_wxyz, c3);
    const SIMD4F c2_wxyz_x_c3_yzwx = SIMD4F_MUL(c2_wxyz, c3_yzwx);
    const SIMD4F c2_wxyz_x_c3_zwxy = SIMD4F_MUL(c2_wxyz, c3_zwxy);

    const SIMD4F ar1 = SIMD4F_SUB(SIMD4F_SHUFFLE_WXYZ(c2_wxyz_x_c3_zwxy), SIMD4F_SHUFFLE_ZWXY(c2_wxyz_x_c3));
    const SIMD4F ar2 = SIMD4F_SUB(SIMD4F_SHUFFLE_ZWXY(c2_wxyz_x_c3_yzwx), c2_wxyz_x_c3_yzwx);
    const SIMD4F ar3 = SIMD4F_SUB(c2_wxyz_x_c3_zwxy, SIMD4F_SHUFFLE_WXYZ(c2_wxyz_x_c3));

    const SIMD4F br1 = SIMD4F_SUB(SIMD4F_SHUFFLE_WXYZ(c0_wxyz_x_c1_zwxy), SIMD4F_SHUFFLE_ZWXY(c0_wxyz_x_c1));
    const SIMD4F br2 = SIMD4F_SUB(SIMD4F_SHUFFLE_ZWXY(c0_wxyz_x_c1_yzwx), c0_wxyz_x_c1_yzwx);
    const SIMD4F br3 = SIMD4F_SUB(c0_wxyz_x_c1_zwxy, SIMD4F_SHUFFLE_WXYZ(c0_wxyz_x_c1));


    const SIMD4F c0_sum = SIMD4F_MUL_ADD(c0_yzwx, ar3,
        SIMD4F_MUL_ADD(c0_zwxy, ar2,
            SIMD4F_MUL(c0_wxyz, ar1)));

    const SIMD4F c1_sum = SIMD4F_MUL_ADD(c1_wxyz, ar1,
        SIMD4F_MUL_ADD(c1_zwxy, ar2,
            SIMD4F_MUL(c1_yzwx, ar3)));

    const SIMD4F c2_sum = SIMD4F_MUL_ADD(c2_yzwx, br3,
        SIMD4F_MUL_ADD(c2_zwxy, br2,
            SIMD4F_MUL(c2_wxyz, br1)));

    const SIMD4F c3_sum = SIMD4F_MUL_ADD(c3_yzwx, br3,
        SIMD4F_MUL_ADD(c3_zwxy, br2,
            SIMD4F_MUL(c3_wxyz, br1)));


    const SIMD4F d0 = SIMD4F_MUL(c1_sum, c0);
    const SIMD4F d1 = SIMD4F_ADD(d0, SIMD4F_MERGE_HIGH(d0, d0));
    const SIMD4F det = SIMD4F_SUB(d1, SIMD4F_SET_Y(d1));

    const SIMD4F invdet = SIMD4F_SET_X(SIMD4F_DIV(SIMD4F_SET(1.0f), det));

    if (invdet.m128_f32[0] == std::numeric_limits<float>::infinity())
    {
        ASSERT_MSG(false,"");
        return SIMD4X4F_IDENTITY();
    }

    const SIMD4F o0 = SIMD4F_MUL(SIMD4F_FLIP_SIGN_0101(c1_sum), invdet);
    const SIMD4F o1 = SIMD4F_MUL(SIMD4F_FLIP_SIGN_1010(c0_sum), invdet);
    const SIMD4F o2 = SIMD4F_MUL(SIMD4F_FLIP_SIGN_0101(c3_sum), invdet);
    const SIMD4F o3 = SIMD4F_MUL(SIMD4F_FLIP_SIGN_1010(c2_sum), invdet);

    const SIMD4X4F mt = SIMD4X4F_CREATE(o0, o1, o2, o3);

    return SIMD4X4F_TRANSPOSE(mt);

}

