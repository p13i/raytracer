#ifndef rt_ray_hpp
#define rt_ray_hpp

#include <cmath>
#include <iostream>

#include "rt_point.hpp"
#include "rt_vector.hpp"

namespace rt {

    struct Ray {
        Point origin;
        Vector direction;

        Ray() : Ray({}, {}) {}

        Ray(Point origin, float radians) : Ray(origin, Vector{{cosf(radians), sinf(radians)}}) {}

        Ray(Point origin, Point towards) : Ray(origin, Vector(towards - origin).unit()) {}

        Ray(Point origin, Vector direction) : origin(origin), direction(direction) {}

        Point operator()(float time) const;

        float radians() const;

    };

}

std::ostream &operator<<(std::ostream &os, const rt::Ray &ray);

#endif /* rt_ray_hpp */
