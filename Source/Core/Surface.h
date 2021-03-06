#ifndef _SURFACE_H_
#define _SURFACE_H_

//  Abstract class Surface may be subclassed to a particular type of 
//  Surface such as a Plane, Sphere, Triangle, and triangle mesh.

#include "Vector3d.h"
#include "Ray.h"
#include "Color.h"
#include "Material.h"


    struct SurfaceHitRecord
    {
        double t;		   // Ray hits at p = Ray.origin() + t * Ray.direction().
        Vector3d p;		   // The point of intersection.
        float u;
        float v;
        Vector3d normal;   // Surface normal at p. May not be unit vector.
        int mat; // Pointer to the surface material.
    };




class Surface 
{
public:
    enum SurfaceType
    {
        SPHERER_SURFACE,
        PLANE_SURFACE,
        TRIANGLE_SURFACE
    };


	int mat;	// Material of the surface.
    SurfaceType type;

	// Does a Ray hit the Surface?
	virtual bool hit( 
					const Ray &r, // Ray being sent.
					double tmin,  // Minimum hit parameter to be searched for.
					double tmax,  // Maximum hit parameter to be searched for.
					SurfaceHitRecord &rec 
                    ) const = 0;


    // Does a Ray hit any Surface?  Allows early termination.
    virtual bool shadowHit(        
					const Ray &r, // Ray being sent.
					double tmin,  // Minimum hit parameter to be searched for.
					double tmax   // Maximum hit parameter to be searched for.
					) const 
	{ 
       SurfaceHitRecord rec;
       return hit( r, tmin, tmax, rec );
    }


}; // Surface


typedef Surface *SurfacePtr;


#endif // _SURFACE_H_
