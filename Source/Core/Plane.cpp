#include <cmath>
#include "Plane.h"

using namespace std;



bool Plane::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
	Vector3d N( A, B, C );
	double NRd = dot( N, r.direction() );
	double NRo = dot( N, r.origin() );
	double t = (-D - NRo) / NRd;
	if ( t < tmin || t > tmax ) return false;

	// We have a hit -- populat hit record. 
	rec.t = t;
	rec.p = r.pointAtParam(t);
	rec.normal = N;
	rec.mat_ptr = matp;
    rec.u = 0.5f;
    rec.v = 0.5f;

	return true;
}



bool Plane::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
	Vector3d N( A, B, C );
	double NRd = dot( N, r.direction() );
	double NRo = dot( N, r.origin() );
	double t = (-D - NRo) / NRd;
	return ( t >= tmin && t <= tmax );
}
