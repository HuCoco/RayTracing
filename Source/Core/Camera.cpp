#include "Camera.h"

void Camera::Initialize(const Vec3f& eye, const Vec3f& lookAt, const Vec3f& up,
    float left, float right, float bottom, float top, float near,
    uint32_t width, uint32_t height)
{
    m_ImageWidth = width;
    m_ImageHeight = height;

    m_Cop = eye;
    Vec3f cop_n = (eye - lookAt).GetNormalized();
    Vec3f cop_u = Vec3f::Cross(up.GetNormalized(), cop_n);
    Vec3f cop_v = Vec3f::Cross(cop_n, cop_u);

    m_ImageOrigin = m_Cop + (left * cop_u) + (bottom * cop_v) + (-near * cop_n);

    m_ImageU = (right - left) * cop_u;
    m_ImageV = (top - bottom) * cop_v;
}

Ray Camera::GetRay(float pixel_x, float pixel_y)
{
    Vec3f imgPos = m_ImageOrigin + (pixel_x / m_ImageWidth) * m_ImageU + (pixel_y / m_ImageHeight) * m_ImageV;
    return Ray(m_Cop, imgPos - m_Cop);
}

