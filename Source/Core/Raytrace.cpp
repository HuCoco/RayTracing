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
using namespace std;


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
								   const Material &mat, const PointLightSource &ptLight )
{
	Vector3d NN = ( dot( L, N ) >= 0.0 )?  N : -N;

	Vector3d R = mirrorReflect( L, NN );
	float NL = (float) dot( NN, L );
	float RVn = pow( (float) dot( V, R ), (float) mat.n );

	return ptLight.I_source * ( mat.k_d * NL  +  mat.k_r * RVn );
}

static Color computeBlinnPhongLighting(const Vector3d &L, const Vector3d &N, const Vector3d &V,
    const Material &mat, const PointLightSource &ptLight)
{
    Vector3d NN = (dot(L, N) >= 0.0) ? N : -N;

    Vector3d R = mirrorReflect(L, NN);
    Vector3d H = L + V;
    H = H.makeUnitVector();
    float NL = (float)dot(NN, L);
    float RVn = pow((float)dot(N, H), (float)mat.n);

    return ptLight.I_source * (mat.k_d * NL + mat.k_r * RVn);
}


static Color computeBRDF(Vector3d L, Vector3d V, Vector3d N, float Roughness, const Material &mat, const PointLightSource &ptLight, float metallic, float attenuation)
{
    Color diffuse = mat.k_d * (1 / PI);
    float NoV = dot(N, V);
    float NoL = dot(N, L);
    Vector3d H = L + V;
    H = H.makeUnitVector();
    float NoH = dot(N, H);

    Vector3d F0 = Vector3d(0.04);
    Vector3d albedo = Vector3d(mat.k_d.r(), mat.k_d.g(), mat.k_d.b());
    F0 = Lerp(F0, albedo, metallic);
    Color F = FresnelEquation(N, V, F0);
    
    float NDF = NormalDistributionFunction(NoH, Roughness);
    float G = GeometryFunction(N, V, L, Roughness);
    Color nominator = NDF * G * F;
    float denominator = 4.0 * Math::max(NoV, 0.0f) * Math::max(NoL, 0.0f) + 0.001f;
    Color specular = nominator / denominator;

    Color kS = F;
    Color kD = Color(1.0,1.0,1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = Math::max(NoL, 0.0f);

    return (kD * mat.k_d / PI + specular) * (attenuation * ptLight.I_source) * NdotL;
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
    int ii = 0;
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
        //result += computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i])* (1.0 / scene.numPtLights);
        //result += computeBlinnPhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
        float attenuation = 1.0 / (MaxLength * MaxLength);
        result += computeBRDF(L, V, N, 0.0, *nearestHitRec.mat_ptr, scene.ptLight[i], 1, attenuation);   
    }
    //***********************************************
    //Color ambient = Color(0.03) * nearestHitRec.mat_ptr->k_d;
    //result = ambient + result;



// Add to result the global ambient lighting.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    result += scene.amLight.I_a * nearestHitRec.mat_ptr->k_a;
    //***********************************************





// Add to result the reflection of the scene.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    if(reflectLevels > 0)
    {
        Vector3d dir = mirrorReflect(V, N);
        Ray rRay(nearestHitRec.p,dir);
        result += nearestHitRec.mat_ptr->k_rg * TraceRay(rRay,scene,--reflectLevels,hasShadow);
    }
    //***********************************************



	return result;
}


