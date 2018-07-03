//============================================================
// STUDENT NAME: HU KE
// NUS User ID.: NUSRI1613
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Raytrace.cpp


#include <cmath>
#include <cfloat>
#include "Vector3d.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Surface.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Scene.h"
#include "Raytrace.h"
#include "BRDF_Function.h"
#include "Texture.h"
using namespace std;

extern Texture tex;
extern Texture lut_tex;
// This is for avoiding the "epsilon problem" or the shadow acne problem.
#define DEFAULT_TMIN	10e-6		

// Use this for tmax for non-shadow ray intersection test.
#define DEFAULT_TMAX	DBL_MAX



//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////

static Vector3d mirrorReflect( const Vector3d &L, const Vector3d &N )
{
	return ( 2.0 * dot( N, L ) ) * N - L;
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting( const Vector3d &L, const Vector3d &N, const Vector3d &V,
								   const EMPMaterial* mat, const PointLightSource &ptLight )
{
	Vector3d NN = ( dot( L, N ) >= 0.0 )?  N : -N;

	Vector3d R = mirrorReflect( L, NN );
	float NL = (float) dot( NN, L );
	float RVn = pow( (float) dot( V, R ), (float) mat->n );

	return ptLight.I_source * ( mat->k_d * NL  +  mat->k_r * RVn );
}

static Color computeBlinnPhongLighting(const Vector3d &L, const Vector3d &N, const Vector3d &V,
    const EMPMaterial* mat, const PointLightSource &ptLight)
{
    Vector3d NN = (dot(L, N) >= 0.0) ? N : -N;

    Vector3d R = mirrorReflect(L, NN);
    Vector3d H = L + V;
    H = H.makeUnitVector();
    float NL = (float)dot(NN, L);
    float RVn = pow((float)dot(N, H), (float)mat->n);

    return ptLight.I_source * (mat->k_d * NL + mat->k_r * RVn);
}


static Color computeBRDF(Vector3d L, Vector3d V, Vector3d N,  const PBRMaterial* mat, const PointLightSource &ptLight, float attenuation)
{
    float NoV = dot(N, V);
    float NoL = dot(N, L);
    Vector3d H = V + L;
    H = H.makeUnitVector();
    float NoH = dot(N, H);

    Vector3d F0 = Vector3d(mat->roughness);
    Vector3d albedo = Vector3d(mat->albedo.r(), mat->albedo.g(), mat->albedo.b());
    F0 = Lerp(F0, albedo, mat->metallic);
    Color F = FresnelEquation(N, V, F0);
    
    float NDF = NormalDistributionFunction(NoH, mat->roughness);
    float G = GeometryFunction(N, V, L, mat->roughness);
    Color nominator = NDF * G * F;
    float denominator = 4.0 * Math::max(NoV, 0.0f) * Math::max(NoL, 0.0f) + 0.001f;
    Color specular = nominator / denominator;

    Color kS = F;
    Color kD = Color(1.0) - kS;
    kD *= (1.0 - mat->metallic);
    float NdotL = Math::max(NoL, 0.0f);

    return (kD * mat->albedo / PI + specular) *(attenuation * ptLight.I_source) * NdotL;
}



//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// reflectLevels: specfies number of levels of reflections (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////

Color Raytrace::TraceRay( const Ray &ray, const Scene &scene, 
					      int reflectLevels, bool hasShadow )
{
	Ray uRay( ray );
	uRay.makeUnitDirection();  // Normalize ray direction.


// Find whether and where the ray hits some surface. 
// Take the nearest hit point.

	bool hasHitSomething = false;
	double nearest_t = DEFAULT_TMAX;
	SurfaceHitRecord nearestHitRec;

	for ( int i = 0; i < scene.numSurfaces; i++ )
	{
		SurfaceHitRecord tempHitRec;
		bool hasHit = scene.surfacep[i]->hit( uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec );

		if ( hasHit && tempHitRec.t < nearest_t )
		{
			hasHitSomething = true;
			nearest_t = tempHitRec.t;
			nearestHitRec = tempHitRec;
		}
	}

	if ( !hasHitSomething ) return scene.backgroundColor;

	nearestHitRec.normal.makeUnitVector();
	Vector3d N = nearestHitRec.normal;	// Unit vector.
	Vector3d V = -uRay.direction();		// Unit vector.
    
	Color result( 0.0f, 0.0f, 0.0f );	// The result will be accumulated here.


	////////////////////////////////////
	//result = nearestHitRec.mat_ptr->k_d; // REMOVE THIS LINE AFTER YOU HAVE FINISHED CODE BELOW.
	////////////////////////////////////


// Add to result the phong lightin contributed by each point light source.
// Compute shadow if hasShadow is true.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    
    float shadow = 1.0f;
    for(int i = 0 ; i < scene.numPtLights ; i++)
    {
        
        Vector3d Lin = scene.ptLight[i].position - nearestHitRec.p;
        double MaxLength = (Lin).length();
        double invLen = 1 / MaxLength;
        Vector3d L = (Lin)*invLen;

        if(hasShadow)
        {
            Ray newRay(nearestHitRec.p,L);
            for(int j = 0 ; j < scene.numSurfaces ; j++)
            {
                if(scene.surfacep[j]->shadowHit(newRay, DEFAULT_TMIN, MaxLength))
                {
                    shadow = 0.0f;
                    break;
                }
            }
            if(shadow == 0.0f)
            {
                continue;
            }
        }

        if (nearestHitRec.mat_ptr->type == EMP)
        {
            result += computePhongLighting(L, N, V, reinterpret_cast<EMPMaterial*>(nearestHitRec.mat_ptr), scene.ptLight[i]);
            //result += computeBlinnPhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
        }
        else if(nearestHitRec.mat_ptr->type == PBR)
        {
            float attenuation = 1.0 / (MaxLength * MaxLength);
            result += computeBRDF(L, V, N, reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr), scene.ptLight[i], attenuation);
        }

    }
    //***********************************************



// Add to result the global ambient lighting.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    if (nearestHitRec.mat_ptr->type == EMP)
    {
        result /= scene.numPtLights;
        result += scene.amLight.I_a * reinterpret_cast<EMPMaterial*>(nearestHitRec.mat_ptr)->k_a;
    }
    else if (nearestHitRec.mat_ptr->type == PBR)
    {
        //Color ambient = Color(0.03) * reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->albedo;
        //result += ambient;
        //result = result / (result + Color(1.0f));
        //result.gammaCorrect();
    }
    //***********************************************





// Add to result the reflection of the scene.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    if(reflectLevels > 0)
    {
        Vector3d dir = mirrorReflect(V, N);
        Ray rRay(nearestHitRec.p, dir);
        if (nearestHitRec.mat_ptr->type == EMP)
        {
            result += reinterpret_cast<EMPMaterial*>(nearestHitRec.mat_ptr)->k_rg * TraceRay(rRay, scene, --reflectLevels, hasShadow);
        }
        else if (nearestHitRec.mat_ptr->type == PBR)
        {
            float NoV = dot(N, V);
            Color envBRDF = lut_tex.GetPixel(NoV, reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->roughness);
            Vector3d F0 = Vector3d(reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->roughness);
            Vector3d albedo = Vector3d(reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->albedo.r(), reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->albedo.g(), reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->albedo.b());
            F0 = Lerp(F0, albedo, reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->metallic);
            Color F = FresnelEquation(N, V, F0);

            Color kS = F;
            Color kD = Color(1.0, 1.0, 1.0) - kS;
            kD *= 1.0 - reinterpret_cast<PBRMaterial*>(nearestHitRec.mat_ptr)->metallic;


            result += (kD * envBRDF.r() + envBRDF.g()) * TraceRay(rRay, scene, --reflectLevels, hasShadow);
        }

    }
    //***********************************************



	return result;
}


