#include "rt_linesegment.hpp"

std::ostream& operator << (std::ostream& os, const rt::LineSegment& edge)
{
    os << "LineSegment(";
    os << edge.a;
    os << ", " << edge.b << ')';
    return os;
}

bool rt::LineSegment::operator == (LineSegment other) {
    return this->a == other.a && this->b == other.b;
}

bool rt::LineSegment::operator != (LineSegment other) {
    return !(*this == other);
}
