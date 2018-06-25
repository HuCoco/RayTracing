#version 430 core

#include "Geometry.h"

layout (local_size_x = 16, local_size_y = 16) in; 

layout (rgb32f, binding = 0) uniform image2D output_image; 

uniform PBRMaterial g_MaterialList[32]
uniform uint g_NumActiveMaterials;

uniform Sphere g_SphereList[32];
uniform uint g_NumActiveSpheres;

uniform uint NumSample;
uniform uint ReflectLevels;
uniform bool HasShadow;

float RadicalInverse(int bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10f;
}

vec2 Hammersley(uint32_t i, uint32_t N) 
{
    return vec2(float(i) / float(N),RadicalInverse(i));
}

vec3 RayTrace(Ray ray, int reflectLevels, bool hasShadow)

void main(void)
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	vec3 color(0,0,0);
	for(uint i = 0 ; i < NumSample; i++)
	{
		vec2 offset = Hammersley(i, NumSample);
		vec2 pixelPos((float)(pos.x) + offset.x,(float)(pos.y) + offset.y);
		Ray ray; // create ray from camera
		color += RayTrace(ray,ReflectLevels,HasShadow);
	}
	color /= NumSample;
	clamp(color,0,1);
	imageStore(output_image, pos.xy, color); 
}
