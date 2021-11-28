//
//  rt_linesegment.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_linesegment.hpp"

std::ostream& operator << (std::ostream& os, const rt::LineSegment& edge)
{
    os << "LineSegment(";
    os << edge.a;
    os << ", " << edge.b << ')';
    return os;
}

rt::Point rt::LineSegment::midpoint() {
    return {(a.x + b.x) / 2.f, (a.y + b.y) / 2.f};
}

bool rt::LineSegment::operator == (LineSegment other) {
    return this->a == other.a && this->b == other.b;
}

bool rt::LineSegment::operator != (LineSegment other) {
    return !(*this == other);
}
