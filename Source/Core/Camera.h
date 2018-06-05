#pragma once
#include <Math/Vec3f.h>
#include <Core/Ray.h>

class Camera
{
public:
    Camera() = default;
    void Initialize(const Vec3f& eye, const Vec3f& lookAt, const Vec3f& up,
        float left, float right, float bottom, float top, float near,
        uint32_t width, uint32_t height);


    Ray GetRay(float pixel_x, float pixel_y);
private:
    Vec3f m_Cop{ 0.0f,0.0f,0.0f };
    Vec3f m_ImageOrigin{ 0.0f,0.0f,0.0f };
    Vec3f m_ImageU{ 0.0f,0.0f,0.0f };
    Vec3f m_ImageV{ 0.0f,0.0f,0.0f };
    uint32_t m_ImageWidth{ 0 };
    uint32_t m_ImageHeight{ 0 };
};