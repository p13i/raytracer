//
//  rt_ray.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_ray_hpp
#define rt_ray_hpp

#include "rt_point.hpp"
#include "rt_vector.hpp"

#include <cmath>
#include <iostream>

namespace rt {

struct Ray {
    rt::Point origin;
    rt::Vector direction;
    
    Ray(): Ray({}, {}) { }
    
    Ray(rt::Point origin, float radians): Ray(origin, Vector{{cos(radians), sin(radians)}}) { }
    
    Ray(rt::Point origin, rt::Point towards): Ray(origin, Vector(towards - origin).unit()) { }
    
    Ray(rt::Point origin, rt::Vector direction): origin(origin), direction(direction) { }
    
    rt::Point operator () (float time) {
        rt::Vector atTime = direction.unit() * time;
        return origin + (direction * time).dest;
    }
    
    bool intersects(rt::Point p) {
        // p is only on this ray if these normals are equal:
        return rt::Vector(p - origin).unit() == direction.unit();
    }
    
    float radians() {
        auto dir = direction.unit().dest;
        float theta = atan(dir.y / dir.x);
        
        // Source: https://math.stackexchange.com/a/1183407/414919
        if (dir.x < 0) {
            theta += M_PI;
        }
        
        return theta;
    }
    
    
};

}

std::ostream& operator << (std::ostream& os, const rt::Ray& ray);

#endif /* rt_ray_hpp */
