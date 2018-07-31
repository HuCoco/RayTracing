//============================================================
// STUDENT NAME: HU KE
// NUS User ID.: NUSRI1613
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Sphere.cpp



#include <cmath>
#include "Sphere.h"

using namespace std;



bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    Vector3d Rd = r.direction();
    Vector3d Ro = r.origin() - center;
    double a = dot(Rd,Rd);
    double b = 2.0 * dot(Rd, Ro);
    double c = dot(Ro,Ro) - pow(radius, 2);
    double d = pow(b, 2) - 4.0 * a * c;
    if(d < 0)
    {
        return false;
    }
    double t = (-b - sqrt(d)) / (2.0f * a);
    if ( t >= tmin && t <= tmax )
    {
        rec.t = t ;
        rec.p = r.pointAtParam(t);
        rec.normal = (Ro + t * Rd) / radius;
        rec.mat = mat;
        rec.u = std::atan2(rec.p.x(), rec.p.z()) / (3.1415926f * 2.0f);
        rec.v = 1.0f - (std::acos(rec.p.y()) / (3.1415926f));
        return true;
    }
    
    //***********************************************
    return false; // You can remove/change this if needed.
}




bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    Vector3d Rd = r.direction();
    Vector3d Ro = r.origin() - center;
    double a = dot(Rd,Rd);
    double b = 2 * dot(Rd, Ro);
    double c = dot(Ro,Ro) - pow(radius, 2);
    double d = pow(b, 2) - 4 * a * c;
    if(d < 0)
    {
        return false;
    }
    double t = (-b - sqrt(d)) / (2 * a);

    //***********************************************

	return (t >= tmin && t <= tmax); // You can remove/change this if needed.
}

