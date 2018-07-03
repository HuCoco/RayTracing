#version 430 core

//#include "Geometry.h"

#define MAX_NUM_POINT_LIGHTS 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_SPHERES 32

layout (local_size_x = 16, local_size_y = 16) in; 

layout (rgba32f, binding = 0) uniform image2D output_image; 


struct DirectionLightData
{
	vec3 color;
};

struct PointLightData
{
	vec3 position;
	vec3 color;
};

struct MaterialData
{
	vec3 albedo;
	float metallic;
    float roughness;
};

struct SphereData
{
	vec3 center;
	float radius;
	int mat;
};

struct Ray
{
	vec3 origin;
	vec3 direction;
};

struct CameraData
{
	vec3 COP;
	vec3 ImageOrigin;
	vec3 ImageU;
	vec3 ImageV;
	int ImageWidth;
	int ImageHeight;
};

struct SurfaceHitRecord
{
	float t;
	vec3 p;
	vec3 n;
	int mat;
};
SurfaceHitRecord nearestHitRec;
SurfaceHitRecord tempHitRec;

layout(std140, binding = 0) uniform DirectionLight
{
	DirectionLightData DirectionLights;
};

layout(std140, binding = 1) uniform PointLight
{
	PointLightData PointLights[MAX_NUM_POINT_LIGHTS];
};
uniform uint NumActiveLights;

layout(std140, binding = 2) uniform Material
{
	MaterialData Materials[MAX_NUM_MATERIALS];
};
uniform uint NumActiveMaterials;

layout(std140, binding = 3) uniform Sphere
{
	SphereData Spheres[MAX_NUM_SPHERES];
};
uniform uint NumActiveSpheres;

layout(std140, binding = 4) uniform Camera
{
	CameraData Cameras;
};

uniform uint NumSample;
uniform uint ReflectLevels;
uniform bool HasShadow;

Ray getRay(float x, float y)
{
	vec3 imgPos = Cameras.ImageOrigin + (x/Cameras.ImageWidth) * Cameras.ImageU + (y/Cameras.ImageHeight) * Cameras.ImageV;
	return Ray(Cameras.COP,imgPos - Cameras.COP);
}

float RadicalInverse(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10f;
}

vec2 Hammersley(uint i, uint N) 
{
    return vec2(float(i) / float(N),RadicalInverse(i));
}

vec3 FresnelEquation(vec3 N, vec3 V, vec3 F)
{
    float NoV = dot(N, V);
    vec3 res = F + (vec3(1.0f) - F) * pow(1.0f - NoV, 5);
    return res;
}

float GeometrychlickGGX(float NoV, float k)
{
    float nom = NoV;
    float denom = NoV * (1.0 - k) + k;

    return nom / denom;
}

float GeometryFunction(vec3 N, vec3 V, vec3 L, float Roughness)
{
    float k = pow(Roughness + 1, 2) / 8.0f;
    float NoV = max(dot(N, V), 0.0);
    float NoL = max(dot(N, L), 0.0);
    float ggx1 = GeometrychlickGGX(NoV, k);
    float ggx2 = GeometrychlickGGX(NoL, k);

    return ggx1 * ggx2;
}

float NormalDistributionFunction(float NoH, float Roughness)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float NdotH = max(NoH, 0.0f);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.141592 * denom * denom;

    return nom / denom;
}

vec4 computeBRDF(vec3 L, vec3 V, vec3 N, MaterialData mat, PointLightData plight, float attenuation)
{
	float NoV = dot(N, V);
	float NoL = dot(N, L);
	vec3 H = V + L;
	H = normalize(H);
	float NoH = dot(N, H);

	vec3 F0 = vec3(mat.roughness);
	vec3 albedo = mat.albedo;
	F0 = mix(F0, albedo, vec3(mat.metallic));

	vec3 F = FresnelEquation(N, V, F0);

	float NDF = NormalDistributionFunction(NoH, mat.roughness);
    float G = GeometryFunction(N, V, L, mat.roughness);

    vec3 nominator = NDF * G * F;
   	float denominator = 4.0 * max(NoV,0.0) * max(NoL, 0.0) + 0.0001;
   	vec3 specular = nominator / denominator;

   	vec3 kS = F;
   	vec3 kD = vec3(1.0) - kS;
   	kD =  kD * vec3(1.0 - mat.metallic);
   	float NdotL = max(NoL, 0.0);

   	vec3 res = (kD * mat.albedo / 3.1415926 + specular) *(attenuation * plight.color) * NdotL;
   	return vec4(res,1.0);
}


bool SphereHit(SphereData sphere, Ray ray, float tmin, float tmax, bool onlyCheckShadow)
{
	vec3 Rd = ray.direction;
	vec3 Ro = ray.origin - sphere.center;

	float a = dot(Rd,Rd);
    float b = 2.0 * dot(Rd, Ro);
    float c = dot(Ro,Ro) - pow(sphere.radius, 2);
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
	    	tempHitRec.t = t;
	    	tempHitRec.p = ray.origin + t * ray.direction;
	    	tempHitRec.n = (Ro + t * Rd) / sphere.radius;
	    	tempHitRec.mat = sphere.mat;
    	}
    	return true;
    }
    return false;
}

const float DEFAULT_TMAX = 3.402823466e+38F;
const float DEFAULT_TMIN = 1.175494351e-38F;

vec4 RayTrace(Ray ray, uint reflectLevels, bool hasShadow)
{
	vec4 color = vec4(0.0);
	bool hasHitSomething = false;
	ray.direction = normalize(ray.direction);
	float nearest_t = DEFAULT_TMAX;
	for(int i = 0 ; i < NumActiveSpheres ; i++)
	{
		bool hasHit = SphereHit(Spheres[i],ray,DEFAULT_TMIN,DEFAULT_TMAX,false);
		if(hasHit && tempHitRec.t < nearest_t)
		{
			hasHitSomething = true;
			nearest_t = tempHitRec.t;
			nearestHitRec = tempHitRec;
		}

	}

	if(!hasHitSomething)
	{
		return color;
	}

	vec3 N = normalize(nearestHitRec.n);
	vec3 V = -ray.direction;

	bool shadow = true;
	for(int i = 0; i < NumActiveLights; i++)
	{
		vec3 Lin = PointLights[i].position - nearestHitRec.p;
		float MaxLength = length(Lin);
		float invLen = 1 / MaxLength;
		vec3 L = Lin * invLen;

		Ray newRay;
		newRay.origin = nearestHitRec.p;
		newRay.direction = L;


        if(HasShadow)
        {
			for(int j = 0; j < NumActiveSpheres; j++)
			{
				bool hasHit = SphereHit(Spheres[j],newRay,DEFAULT_TMIN,MaxLength,true);
				if(hasHit)
				{
					shadow = false;
					break;
				}
			}

			if(shadow == false)
			{
				continue;
			}
		}

		float attenuation = 1.0 /  (MaxLength * MaxLength);
		color += computeBRDF(L,V,N,Materials[nearestHitRec.mat],PointLights[i],attenuation);
	}

	vec4 ambient = vec4(0.03f) *  vec4(Materials[nearestHitRec.mat].albedo,1.0);
	color += ambient;
	color = color / (color + vec4(1.0));

	color = pow(color, vec4(1 / 2.2));

	color.a = 1;
	return color;
}

void main(void)
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	vec4 color = vec4(0.0);
	for(uint i = 0 ; i < NumSample; i++)
	{
		vec2 offset = Hammersley(i, NumSample);
		Ray ray = getRay(float(pos.x) + offset.x, float(pos.y) + offset.y); // create ray from camera
		color += RayTrace(ray,ReflectLevels,HasShadow);
	}
	color /= NumSample;
	clamp(color,0,1);
	barrier();
	color.a = 1;
	imageStore(output_image, pos.xy, color); 
	// vec3 c = DirectionLights.color;
	// ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	
	// vec4 color = vec4(c, 1.0f);
	// imageStore(output_image, pos.xy, color);
}
