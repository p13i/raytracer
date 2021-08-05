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
