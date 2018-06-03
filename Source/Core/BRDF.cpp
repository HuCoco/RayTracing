#include "BRDF.h"

// Hammersley Sampler
float RadicalInverse(uint32_t bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10f;
}

Vec2f Hammersley(uint32_t i, uint32_t N)
{
    return Vec2f(float(i) / float(N), RadicalInverse(i));
}


// Image-Based Lighting
Vec3f ImportanceSampleGGX(Vec2f Xi, float Roughness, Vec3f N)
{
    float a = Roughness * Roughness;
    float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);

    Vec3f H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;

    Vec3f UpVector = abs(N.z) < 0.999 ? Vec3f(0, 0, 1) : Vec3f(1, 0, 0);
    Vec3f TangentX = (Vec3f::Cross(UpVector, N)).GetNormalized();
    Vec3f TangentY = Vec3f::Cross(N, TangentX);
    // Tangent to world space
    return TangentX * H.x + TangentY * H.y + N * H.z;
}

float G_Smith(float Roughness, float NoV, float NoL)
{
    float k = ((Roughness + 1.0f) * (Roughness + 1.0f)) / 8.0;
    float Gv = NoV / (NoV * (1.0f - k) + k);
    float Gl = NoL / (NoL * (1.0f - k) + k);
    return Gl * Gv;
}

Vec3f SpecularIBL(Vec3f SpecularColor, float Roughness, Vec3f N, Vec3f V)
{
    Vec3f SpecularLighting = 0;
    const uint32_t NumSamples = 1024;
    for (uint32_t i = 0; i < NumSamples; i++)
    {
        Vec2f Xi = Hammersley(i, NumSamples);
        Vec3f H = ImportanceSampleGGX(Xi, Roughness, N);
        Vec3f L = H * 2 * Vec3f::Dot(V, H) - V;
        float NoV = Math::clamp(Vec3f::Dot(N, V), 0.0f, 1.0f);
        float NoL = Math::clamp(Vec3f::Dot(N, L), 0.0f, 1.0f);
        float NoH = Math::clamp(Vec3f::Dot(N, H), 0.0f, 1.0f);
        float VoH = Math::clamp(Vec3f::Dot(V, H), 0.0f, 1.0f);
        if (NoL > 0)
        {
            Vec3f SampleColor;
            //Vec3f SampleColor = EnvMap.SampleLevel(EnvMapSampler, L, 0).rgb;
            float G = G_Smith(Roughness, NoV, NoL);
            float Fc = pow(1 - VoH, 5);
            Vec3f F = SpecularColor * (1 - Fc) + Fc;
            // Incident light = SampleColor * NoL
            // Microfacet specular = D*G*F / (4*NoL*NoV)
            // pdf = D * NoH / (4 * VoH)
            SpecularLighting += SampleColor * F * G * VoH / (NoH * NoV);
        }
    }
    return SpecularLighting / NumSamples;
}

Vec3f PrefilterEnvMap(float Roughness, Vec3f N, Vec3f V)
{
    Vec3f PrefilteredColor;
    float TotalWeight = 0;
    const uint32_t NumSamples = 1024;
    for (uint32_t i = 0; i < NumSamples; i++)
    {
        Vec2f Xi = Hammersley(i, NumSamples);
        Vec3f H = ImportanceSampleGGX(Xi, Roughness, N);
        Vec3f L = H * 2 * Vec3f::Dot(V, H) - V;
        float NoL = Math::clamp(Vec3f::Dot(N, L), 0.0f, 1.0f);
        if (NoL > 0)
        {
            PrefilteredColor = Vec3f(1.0f, 0.0f, 0.0f) * NoL;
            //PrefilteredColor += EnvMap.SampleLevel(EnvMapSampler, L, 0).rgb * NoL;
            TotalWeight += NoL;
        }
    }
    return PrefilteredColor / TotalWeight;
}


Vec2f IntegrateBRDF(float Roughness, Vec3f N, Vec3f V)
{
    auto asd = Vec3f::Dot(N, V);
    float NoV = Math::clamp(Vec3f::Dot(N, V), 0.0f, 1.0f);

    //Vec3f V;
    //V.x = sqrt(1.0f - NoV * NoV); // sin
    //V.y = 0;
    //V.z = NoV; // cos
    float A = 0;
    float B = 0;
    const uint32_t NumSamples = 1024;
    for (uint32_t i = 0; i < NumSamples; i++)
    {
        Vec2f Xi = Hammersley(i, NumSamples);
        Vec3f H = ImportanceSampleGGX(Xi, Roughness, N);
        Vec3f L = H * 2 * Vec3f::Dot(V, H) - V;
        float NoL = Math::clamp((L.z), 0.0f, 1.0f);
        float NoH = Math::clamp((H.z), 0.0f, 1.0f);
        float VoH = Math::clamp((Vec3f::Dot(V, H)), 0.0f, 1.0f);
        if (NoL > 0)
        {
            // issue.
            float G = G_Smith(Roughness, NoV, NoL);
            float G_Vis = G * VoH / (NoH * NoV);
            float Fc = pow(1 - VoH, 5);
            A += (1 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    return Vec2f(A, B) / NumSamples;
}

Vec3f ApproximateSpecularIBL(Vec3f SpecularColor, float Roughness, Vec3f N, Vec3f V)
{
    float NoV = Math::clamp(Vec3f::Dot(N, V), 0.0f, 1.0f);
    Vec3f R = N * 2 * Vec3f::Dot(V, N) - V;
    Vec3f PrefilteredColor = PrefilterEnvMap(Roughness, N, V);
    Vec2f EnvBRDF = IntegrateBRDF(Roughness, N, V);
    return PrefilteredColor * (SpecularColor * EnvBRDF.x + EnvBRDF.y);
}
