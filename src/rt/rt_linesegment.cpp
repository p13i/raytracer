#include "rt_linesegment.hpp"

std::ostream& operator<<(std::ostream& os,
                         const rt::LineSegment& edge) {
  return os << "LineSegment(" << edge.a << ", " << edge.b
            << ')';
}

bool operator==(const rt::LineSegment& this_,
                const rt::LineSegment& other) {
  return this_.a == other.a && this_.b == other.b;
};

bool operator!=(const rt::LineSegment& this_,
                const rt::LineSegment& other) {
  return !(this_ == other);
}