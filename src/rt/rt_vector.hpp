//
//  rt_vector.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_vector_hpp
#define rt_vector_hpp

#include <ostream>

#include "rt_point.hpp"
#include "rt_linesegment.hpp"
#include "rt_math.hpp"

namespace rt {

struct Vector {

    rt::Point origin, dest;  // from, to
    
    Vector(rt::Point dest): origin({0, 0}), dest(dest) { }
    
    Vector(rt::Point orig, rt::Point dest): origin(orig), dest(dest) { }
    
    rt::Point _direction() {
        return dest - origin;
    }
    
    Vector unit() {
        return {_direction() / magnitude()};
    }
    
    rt::LineSegment lineSegment() {
        return {origin, dest};
    }
    
    float magnitude() {
        auto dir = _direction();
        return rt::math::sqrt(dir.x * dir.x + dir.y * dir.y);
    }
    
    Vector operator * (float scalar) {
        auto newDest = origin + (dest - origin) * scalar;
        return Vector(origin, newDest);
    }
    
    bool operator == (Vector other) {
        return origin == other.origin && dest == other.dest;
    }
    
    Vector operator - (Vector other) {
        return Vector(origin - other.origin, dest - other.dest);
    }
};

}

rt::Vector operator * (int scalar, rt::Vector vec);

rt::Vector operator * (float scalar, rt::Vector vec);

std::ostream& operator << (std::ostream& os, const rt::Vector& vec);

#endif /* rt_vector_hpp */
