#include "rt_beam.hpp"

#include "rt_geo.hpp"
#include "rt_math.hpp"
#include "rt_settings.hpp"
#include "rt_unbound_beam.hpp"

using namespace rt;

Point rt::Origin(const UnboundBeam u_beam) {
  const Ray bound_a_reverse =
      geo::rotate(u_beam.bound_a_, RT_PI);
  const Ray bound_b_reverse =
      geo::rotate(u_beam.bound_b_, RT_PI);
  Point intersection;
  geo::intersection_of_rays(bound_a_reverse,
                            bound_b_reverse, intersection);
  return intersection;
}

bool rt::Contains(UnboundBeam u_beam,
                  LineSegment line_segment) {
  // Contains if line_segment intersects either infinite
  // bound or if a line_segment's point is between the
  // bounds via the angle from the origin.
  Point this_origin = Origin(u_beam);

  // Intersects left bound
  LineSegment line_left{this_origin,
                        u_beam.bound_a_(RT_SMALL_INF)};
  Point _;
  bool intersects_left_bound =
      geo::intersection(line_segment, line_left, _);
  if (intersects_left_bound) {
    return true;
  }

  LineSegment line_right{this_origin,
                         u_beam.bound_a_(RT_SMALL_INF)};
  bool intersects_right_bound =
      geo::intersection(line_segment, line_right, _);
  if (intersects_right_bound) {
    return true;
  }

  float bound_a_rad = u_beam.bound_a_.radians();
  float bound_b_rad = u_beam.bound_b_.radians();

  if (bound_a_rad > bound_b_rad) {
    float temp = bound_a_rad;
    bound_a_rad = bound_b_rad;
    bound_b_rad = temp;
  }

  float angle_to_point_a =
      Ray(this_origin, line_segment.a).radians();
  if (bound_a_rad <= angle_to_point_a &&
      angle_to_point_a <= bound_b_rad) {
    return true;
  }

  float angle_to_point_b =
      Ray(this_origin, line_segment.b).radians();
  if (bound_a_rad <= angle_to_point_b &&
      angle_to_point_b <= bound_b_rad) {
    return true;
  }

  return false;
}

bool operator==(const Beam &first, const Beam &second) {
  return (first.mBoundA == second.mBoundA) &&
         (first.mBoundB == second.mBoundB);
}

bool operator!=(const Beam &first, const Beam &second) {
  return first.mBoundA != second.mBoundA ||
         first.mBoundB != second.mBoundB;
}

bool operator==(const UnboundBeam &first,
                const UnboundBeam &second) {
  return (first.bound_a_ == second.bound_a_) &&
         (first.bound_b_ == second.bound_b_);
}

bool operator!=(const UnboundBeam &first,
                const UnboundBeam &second) {
  return first.bound_a_ != second.bound_a_ ||
         first.bound_b_ != second.bound_b_;
}

std::ostream &operator<<(std::ostream &os,
                         const UnboundBeam &u_beam) {
  return os << "UnboundBeam(a=" << u_beam.bound_a_
            << ", b=" << u_beam.bound_b_ << ")";
}

std::ostream &operator<<(std::ostream &os,
                         const Beam &beam) {
  return os << "Beam(a=" << beam.mBoundA
            << ", b=" << beam.mBoundB << ")";
}