//
//  rt_point.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_point_hpp
#define rt_point_hpp

#include <ostream>

#include "rt_math.hpp"

namespace rt {

struct Point {
    float x, y;
    
    Point(): Point(0, 0) { }
    
    Point(float x, float y): x(x), y(y) { }
    
    bool operator == (Point other);
    
    bool operator != (Point other);
    
    Point operator + (Point other) {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator - (Point other) {
        return Point(x - other.x, y - other.y);
    }
    
    Point operator * (float scalar) {
        return Point(x * scalar, y * scalar);
    }
    
    Point operator / (float scalar) {
        return Point(x / scalar, y / scalar);
    }
    
    // Source: https://stackoverflow.com/a/41648840
    // Required for use in std::set
    bool operator < (Point other) const {
      return (x < other.x) || ((!(other.x < x)) && (y < other.y));
    }
    
    float magnitude() {
      return rt::math::sqrt(x * x + y * y);
    }
    
    bool fequal(Point other, float threshold = RT_EPSILON);
};

}

std::ostream& operator << (std::ostream& os, const rt::Point& point);

#endif /* rt_point_hpp */
