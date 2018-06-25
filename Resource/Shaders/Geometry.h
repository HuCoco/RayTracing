// Geometry Function

struct PBRMaterial
{
	vec3 albedo;
	float metallic;
    float roughness;
};

struct SurfaceHitRecord
{
	float t;
	vec3 p;
	vec3 n;
	int mat;
};
SurfaceHitRecord g_HitRecord;

struct Ray
{
	vec3 origin;
	vec3 direction;
};

struct Sphere
{
	vec3 center;
	float radius;
	int mat;
};

bool SphereHit(Ray ray, float tmin, float tmax, bool onlyCheckShadow)
{
	vec3 Rd = ray.direction;
	vec3 Ro = ray.origin - center;

	float a = dot(Rd,Rd);
    float b = 2.0 * dot(Rd, Ro);
    float c = dot(Ro,Ro) - pow(radius, 2);
    float d = pow(b, 2) - 4.0 * a * c;
    if(d < 0.0f)
    {
        return false;
    }
    
    float t = (-b - sqrt(d)) / (2.0f * a);
    if ( t >= tmin && t <= tmax )
    {
    	if(onlyCheckShadow == false)
    	{
	    	g_HitRecord.t = t;
	    	g_HitRecord.p = ray.origin + t * ray.direction;
	    	g_HitRecord.normal = (Ro + t * Rd) / radius;
	    	g_HitRecord.mat = mat;
    	}
    	return true;
    }
    return false;
}
