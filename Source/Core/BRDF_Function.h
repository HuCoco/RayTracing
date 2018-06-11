#pragma once
#include <Math/Math.h>
#include <Core/Vector3d.h>
#include <Core/Color.h>
// Physically based shading model
// parameterized with the below options

// Microfacet specular = D*G*F / (4*NoL*NoV) = D*Vis*F
// Vis = G / (4*NoL*NoV)

// D
static Vector3d Diffuse_Lambert(Vector3d DiffuseColor)
{
    return DiffuseColor * (1 / PI);
}

static Vector3d Diffuse_Burley(Vector3d DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
    float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
    float FdV = 1 + (FD90 - 1) * Math::pow(1 - NoV, 5);
    float FdL = 1 + (FD90 - 1) * Math::pow(1 - NoL, 5);
    return DiffuseColor * ((1 / PI) * FdV * FdL);
}

static Vector3d Diffuse_OrenNayar(Vector3d DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
    float a = Roughness * Roughness;
    float s = a;// / ( 1.29 + 0.5 * a );
    float s2 = s * s;
    float VoL = 2 * VoH * VoH - 1;		// double angle identity
    float Cosri = VoL - NoV * NoL;
    float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
    float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * (Cosri >= 0 ? Math::rcp(max(NoL, NoV)) : 1);
    return DiffuseColor / PI * (C1 + C2) * (1 + Roughness * 0.5);
}

static Vector3d Diffuse_Gotanda(Vector3d DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float F0 = 0.04;
    float VoL = 2 * VoH * VoH - 1;		// double angle identity
    float Cosri = VoL - NoV * NoL;
#if 1
    float a2_13 = a2 + 1.36053;
    float Fr = (1 - (0.542026*a2 + 0.303573*a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996*a2*a + 1.50912*a2 - 1.16402*a) * pow(1 - NoV, 1 + Math::rcp(39 * a2*a2 + 1)) + 1);
    //float Fr = ( 1 - 0.36 * a ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( -2.5 * Roughness * ( 1 - NoV ) + 1 );
    float Lm = (Math::max(1 - 2 * a, 0.0f) * (1 - Math::pow(1 - NoL,5)) + Math::min(2 * a, 1.0f)) * (1 - 0.5*a * (NoL - 1)) * NoL;
    float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
    float Bp = Cosri < 0 ? 1.4 * NoV * NoL * Cosri : Cosri;
    float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
    return DiffuseColor / PI * Lr;
#else
    float a2_13 = a2 + 1.36053;
    float Fr = (1 - (0.542026*a2 + 0.303573*a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996*a2*a + 1.50912*a2 - 1.16402*a) * pow(1 - NoV, 1 + rcp(39 * a2*a2 + 1)) + 1);
    float Lm = (max(1 - 2 * a, 0) * (1 - Pow5(1 - NoL)) + min(2 * a, 1)) * (1 - 0.5*a + 0.5*a * NoL);
    float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
    float Bp = Cosri < 0 ? 1.4 * NoV * Cosri : Cosri / max(NoL, 1e-8);
    float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
    return DiffuseColor / PI * Lr;
#endif
}

// D

//float D_Blinn(float Roughness, float NoH)
//{
//    float a = Roughness * Roughness;
//    float a2 = a * a;
//    float n = 2 / a2 - 2;
//    return (n + 2) / (2 * PI) * PhongShadingPow(NoH, n);		// 1 mad, 1 exp, 1 mul, 1 log
//}

static float D_Beckmann(float Roughness, float NoH)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float NoH2 = NoH * NoH;
    return Math::exp((NoH2 - 1) / (a2 * NoH2)) / (PI * a2 * NoH2 * NoH2);
}

static float D_GGX(float Roughness, float NoH)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float d = (NoH * a2 - NoH) * NoH + 1;	// 2 mad
    return a2 / (PI*d*d);					// 4 mul, 1 rcp


}

static float D_GGXaniso(float RoughnessX, float RoughnessY, float NoH, Vector3d H, Vector3d X, Vector3d Y)
{
    float ax = RoughnessX * RoughnessX;
    float ay = RoughnessY * RoughnessY;
    float XoH = dot(X, H);
    float YoH = dot(Y, H);
    float d = XoH * XoH / (ax*ax) + YoH * YoH / (ay*ay) + NoH * NoH;
    return 1 / (PI * ax*ay * d*d);
}

// Vis
static float Vis_Implicit()
{
    return 0.25;
}

static float Vis_Neumann(float NoV, float NoL)
{
    return 1 / (4 * max(NoL, NoV));
}

static float Vis_Kelemen(float VoH)
{
    // constant to prevent NaN
    return Math::rcp(4 * VoH * VoH + 1e-5);
}

static float Vis_Smith(float Roughness, float NoV, float NoL)
{
    float a = Roughness * Roughness;
    float a2 = a * a;

    float Vis_SmithV = NoV + sqrt(NoV * (NoV - NoV * a2) + a2);
    float Vis_SmithL = NoL + sqrt(NoL * (NoL - NoL * a2) + a2);
    return Math::rcp(Vis_SmithV * Vis_SmithL);
}

static float Vis_SmithJointApprox(float Roughness, float NoV, float NoL)
{
    float a = (Roughness) * (Roughness);
    float Vis_SmithV = NoL * (NoV * (1 - a) + a);
    float Vis_SmithL = NoV * (NoL * (1 - a) + a);
    // Note: will generate NaNs with Roughness = 0.  MinRoughness is used to prevent this
    return 0.5 * Math::rcp(Vis_SmithV + Vis_SmithL);
}


// F
static Vector3d F_None(Vector3d SpecularColor)
{
    return SpecularColor;
}

static Vector3d F_Schlick(Vector3d SpecularColor, float VoH)
{
    float Fc = Math::pow(1 - VoH,5);					// 1 sub, 3 mul
    return SpecularColor + (1 - Fc) * SpecularColor;		// 1 add, 3 mad

                                                // Anything less than 2% is physically impossible and is instead considered to be shadowing
    //return Math::clamp(50.0f * (float)SpecularColor.y(),1.0f,1.0f) * Fc + (1 - Fc) * SpecularColor;

}

static Vector3d F_Fresnel(Vector3d SpecularColor, float VoH)
{
    Vector3d SpecularColorSqrt;
    SpecularColorSqrt.setX(Math::sqrt(Math::clamp(SpecularColor.x(), 0.0, 0.99)));
    SpecularColorSqrt.setY(Math::sqrt(Math::clamp(SpecularColor.y(), 0.0, 0.99)));
    SpecularColorSqrt.setZ(Math::sqrt(Math::clamp(SpecularColor.z(), 0.0, 0.99)));

    Vector3d n = (Vector3d(1.0) + SpecularColorSqrt) / (Vector3d(1.0) - SpecularColorSqrt);

    Vector3d g2 = n * n + Vector3d(VoH * VoH) - Vector3d(1.0);
    Vector3d g = Vector3d(Math::sqrt(g2.x()), Math::sqrt(g2.y()), Math::sqrt(g2.z()));

    Vector3d t1 = (g - Vector3d(VoH) / g + Vector3d(VoH));
    Vector3d t2 = ((g + Vector3d(VoH))*VoH - Vector3d(1.0)) / ((g - Vector3d(VoH))*VoH + Vector3d(1.0));
    return 0.5 * t1 * t1 * (1 + t2 * t2);
}


// Microfacet specular = D*G*F / (4*NoL*NoV) = D*Vis*F
// Vis = G / (4*NoL*NoV)

static Vector3d MicrofacetSpecular(Vector3d SpecularColor, float Roughness, Vector3d L, Vector3d V , Vector3d N)
{
    float NoV = dot(N, V);
    float NoL = dot(N, L);
    Vector3d H = L + V;
    float VoH = dot(V, H);
    Vector3d D = Diffuse_Lambert(SpecularColor);
    Vector3d Vis = Vis_SmithJointApprox(Roughness, NoV, NoL);
    Vector3d F = F_Fresnel(SpecularColor, VoH);

    return D * Vis * F;
}