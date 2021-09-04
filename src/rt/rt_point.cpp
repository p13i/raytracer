//
//  rt_point.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_point.hpp"
#include "rt_math.hpp"

bool rt::Point::operator == (rt::Point other) {
    return x == other.x && y == other.y;
}

bool rt::Point::operator != (rt::Point other) {
    return !(*this == other);
}

std::ostream& operator << (std::ostream& os, const rt::Point& point)
{
    os << "Point(" << point.x << ", " << point.y << ')';
    return os;
}

bool rt::Point::fequal(Point other, float threshold) {
    return rt::math::fequal(this->x, other.x, threshold)
        && rt::math::fequal(this->y, other.y, threshold);
}
