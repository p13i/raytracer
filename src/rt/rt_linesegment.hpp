#ifndef RT_LINESEGMENT_HPP
#define RT_LINESEGMENT_HPP

#include "rt_point.hpp"

namespace rt {

// Standard finite line bounded by two points
struct LineSegment {
  Point a, b;

  LineSegment() : LineSegment(Point(), Point()) {}

  LineSegment(Point a, Point b) : a(a), b(b) {}
};

}  // namespace rt

bool operator==(const rt::LineSegment& this_,
                const rt::LineSegment& other);

bool operator!=(const rt::LineSegment& this_,
                const rt::LineSegment& other);

#endif  // RT_LINESEGMENT_HPP