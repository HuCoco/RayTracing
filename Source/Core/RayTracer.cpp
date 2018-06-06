#include "RayTracer.h"

static Vec3f Reflect(const Vec3f& L, const Vec3f& N)
{
    return (2.0f * Vec3f::Dot(N, L)) * N - L;
}

static Vec3f computePhongLighting(const Vec3f &L, const Vec3f &N, const Vec3f &V,
    const Material &mat, const Light &ptLight)
{
    Vec3f NN = (Vec3f::Dot(L, N) >= 0.0) ? N : -N;

    Vec3f R = Reflect(L, NN);
    float NL = (float)Vec3f::Dot(NN, L);
    float RVn = pow((float)Vec3f::Dot(R, V), (float)mat.n);

    return ptLight.color * (mat.k_d * NL + mat.k_r * RVn);
}

Vec3f RayTracer::TraceRay(const Ray& ray, const Scene& scene, uint32_t numRefection)
{
    Ray uRay(ray);
    uRay.Normalized();

    bool hasHitSomething = false;
    float nearest_t = std::numeric_limits<float>::epsilon();
    SurfaceHitResult nearestHitResult;
    
    auto surfaceList = scene.m_surfaces;
    for (auto iter = surfaceList.begin(); iter != surfaceList.end(); iter++)
    {
        SurfaceHitResult  tempResult;
        bool hasHit = (*iter)->Hit(uRay, std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::max(), tempResult);
        if (hasHit && tempResult.t < nearest_t)
        {
            hasHitSomething = true;
            nearest_t = tempResult.t;
            nearestHitResult = tempResult;
        }
    }

    if (!hasHitSomething)
    {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }

    nearestHitResult.normal.Normalized();
    Vec3f N = nearestHitResult.normal;
    Vec3f V = -uRay.GetDirection();

    Vec3f result(0.0f, 0.0f, 0.0f);

    bool shadow = true;
    auto LightList = scene.m_Lights;
    for (auto iter = LightList.begin(); iter != LightList.end(); iter++)
    {
        Vec3f Lin = (*iter).position - nearestHitResult.position;
        float MaxLength = (Lin).GetLength();
        float invLen = 1.0f / MaxLength;
        Vec3f L = (Lin)*invLen;

        {
            Ray newRay(nearestHitResult.position, L);
            auto surfaceList = scene.m_surfaces;
            for (auto iter2 = surfaceList.begin(); iter2 != surfaceList.end(); iter2++)
            {
                if ((*iter2)->ShadowHit(newRay, std::numeric_limits<float>::epsilon(), MaxLength))
                {
                    shadow = false;
                    break;
                }
            }
            if (shadow == false)
            {
                continue;
            }
        }
        result += computePhongLighting(L, N, V, *nearestHitResult.material, (*iter));
        //result += computePhongLighting(...);
        //result += computeBRDF(...);
    }


    if (numRefection > 0)
    {
        Vec3f dir = Reflect(V, N);
        Ray rRay(nearestHitResult.position, dir);
        result += nearestHitResult.material->k_rg * TraceRay(rRay, scene, --numRefection);
    }

    return result;
}

