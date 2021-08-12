//
//  rt_ray.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_ray.hpp"

std::ostream& operator << (std::ostream& os, const rt::Ray& ray) {
    return os << "Ray(origin=" << ray.origin << ", direction=" << ray.direction << ")";
};
