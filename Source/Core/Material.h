#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"


//////////////////////////////////////////////////////////////////////////////
//
// The lighting model used here is a modified version of that on 
// Slide 8 of Lecture 4 (Ray Tracing).
// Here it is computed as
//
//     I_local = I_a * k_a  +  
//               SUM_OVER_ALL_LIGHTS ( I_source * [ k_d * (N.L) + k_r * (R.V)^n ] )
//
// and
//
//     I = I_local  +  k_rg * I_reflected
//
//
// Note that light sources only illuminate the scene, and they do not 
// appear in the rendered image.
//
//////////////////////////////////////////////////////////////////////////////

namespace PBR
{
    struct Material
    {
        Color albedo;
        float metallic;
        float roughness;
    };
}

namespace Phong
{
    struct Material
    {
        Color ambient;
        Color reflection;
        Color diffuse;
        Color specluar;
        float shininess;
    };
}



#endif // _MATERIAL_H_

