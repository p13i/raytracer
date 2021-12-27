#ifndef rt_vector_hpp
#define rt_vector_hpp

#include <ostream>
#include <cmath>

#include "rt_point.hpp"
#include "rt_linesegment.hpp"
#include "rt_math.hpp"

namespace rt {

struct Vector {

    Point origin, dest;  // from, to
    
    Vector(Point dest): origin({0, 0}), dest(dest) { }
    
    Vector(Point orig, Point dest): origin(orig), dest(dest) { }
    
    Point _direction() const {
        return dest - origin;
    }
    
    Vector unit() const {
        return {_direction() / magnitude()};
    }
    
    rt::LineSegment lineSegment() const {
        return {origin, dest};
    }
    
    float magnitude() const {
        auto dir = _direction();
        return sqrtf(dir.x * dir.x + dir.y * dir.y);
    }
    
    Vector operator * (float scalar) const {
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
