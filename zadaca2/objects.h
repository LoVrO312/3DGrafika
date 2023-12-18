#pragma once
#include <cmath>
#include "geometry.h"
#include "ray.h"
#include "material.h"

class Object 
{
  public:
    Material material;
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;  
};

class Sphere : public Object
{
    float r;
    Vec3f c;
      
  public:
    Sphere(const Vec3f &c, const float &r) : c(c), r(r) { }
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    { 
        Object::material = mat;
    }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        Vec3f e_minus_c = ray.origin - c;
        float d2 = ray.direction * ray.direction;
        
        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);
        
        if (disc < 0)
        {
            return false;
        }
        else
        {
            bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
            if (ray_inside_sphere)
            {
                float t1 = (-ray.direction * e_minus_c + sqrt(disc)) / d2;
                t = t1;
            }
            else
            {
                float t2 = (-ray.direction * e_minus_c - sqrt(disc)) / d2;
                t = t2;
            }
            
            Vec3f hit_point = ray.origin + ray.direction * t;
            normal = (hit_point - c).normalize();
            return true;
        }
    }
};

class Cuboid : public Object 
{
    Vec3f min, max;

  public:
    Cuboid(const Vec3f& min, const Vec3f& max) : min(min), max(max) {}
    Cuboid(const Vec3f& min, const Vec3f& max, const Material &mat) : min(min), max(max) 
    {
        Object::material = mat; 
    }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {   
        bool ret;

        Vec3f a = min, b = min, c = min;
        Vec3f d = max, e = max, f = max;

        a.x = max.x;
        b.y = max.y;
        c.z = max.z;

        d.x = min.x;
        e.y = min.y;
        f.z = min.z; 

        

    }

};