#include "rt_beam.hpp"

#include "rt_geo.hpp"

#include <vector>

using namespace rt;

bool operator==(const Beam &first, const Beam &second) {
  return (first.mBoundA == second.mBoundA) && (first.mBoundB == second.mBoundB);
}

bool operator!=(const Beam &first, const Beam &second) {
  return first.mBoundA != second.mBoundA || first.mBoundB != second.mBoundB;
}

Point UnboundBeam::Origin() const {
  const Ray bound_a_reverse = rt::geo::rotate(bound_a_, RT_PI);
  const Ray bound_b_reverse = rt::geo::rotate(bound_b_, RT_PI);
  Point intersection;
  rt::geo::intersection_of_rays(bound_a_reverse, bound_b_reverse, intersection);
  return intersection;
}

bool UnboundBeam::Contains(LineSegment line_segment) const {
  // Contains if line_segment intersects either infinite bound
  // or if a line_segment's point is between the bounds via
  // the angle from the origin.
  Point this_origin = Origin();

  // Intersects left bound
  LineSegment line_left{this_origin, bound_a_(10000)};
  Point _;
  bool intersects_left_bound = rt::geo::intersection(line_segment, line_left, _);
  if (intersects_left_bound) {
    return true;
  }
  
  LineSegment line_right{this_origin, bound_a_(10000)};
  bool intersects_right_bound = rt::geo::intersection(line_segment, line_right, _);
  if (intersects_right_bound) {
    return true;
  }

  float bound_a_rad = bound_a_.radians();
  float bound_b_rad = bound_b_.radians();

  if (bound_a_rad > bound_b_rad)
  {
    float temp = bound_a_rad;
    bound_a_rad = bound_b_rad;
    bound_b_rad = temp;
  }

  float angle_to_point_a = Ray(this_origin, line_segment.a).radians();
  if (bound_a_rad <= angle_to_point_a && angle_to_point_a <= bound_b_rad) {
    return true;
  }
  
  float angle_to_point_b = Ray(this_origin, line_segment.b).radians();
  if (bound_a_rad <= angle_to_point_b && angle_to_point_b <= bound_b_rad) {
    return true;
  }
  
  return false;
}