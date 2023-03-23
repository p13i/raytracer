#ifndef RT_BEAM_H
#define RT_BEAM_H

#include "rt_ray.hpp"
#include "rt_vector.hpp"

namespace rt {

////////////////////////////////////////////////////////////
/// Represents a beam with finite bounds.
struct Beam {
  const Vector mBoundA, mBoundB;

  Beam(const Vector mBoundA, const Vector mBoundB)
      : mBoundA(mBoundA), mBoundB(mBoundB) {}
};

////////////////////////////////////////////////////////////
/// Represents a beam with bounds that extend to infinity.
class UnboundBeam {
public:
  const Ray bound_a_, bound_b_;

  UnboundBeam(const Ray bound_a, const Ray bound_b)
      : bound_a_(bound_a), bound_b_(bound_b) {}
  Point Origin() const;
  bool Contains(LineSegment line_segment) const;
};
} // namespace rt

bool operator==(const rt::Beam &first, const rt::Beam &second);

bool operator!=(const rt::Beam &first, const rt::Beam &second);

#endif // RT_BEAM_H