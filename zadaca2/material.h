#pragma once
#include "geometry.h"

class Material
{
  public:
    Vec3f diffuse_colour;
    float diffuse_coef = 1;
    float reflection_coef = 1;  
    float opacity_coef = 0;
    
    float specular_coef = 1;
    float phong_exp = 1000;
    
    Material() : diffuse_colour(Vec3f(0, 0, 0)) { }
    Material(const Vec3f &colour) : diffuse_colour(colour) { }
    Material(const Vec3f &colour, float refl_coef, float opacity) : diffuse_colour(colour), reflection_coef(refl_coef), opacity_coef(opacity) { }
};