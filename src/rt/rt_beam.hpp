#ifndef RT_BEAM_H
#define RT_BEAM_H

#include "rt_ray.hpp"
#include "rt_vector.hpp"

namespace rt {

////////////////////////////////////////////////////////////
/// Represents a beam with finite bounds.
struct Beam {
  Vector mBoundA, mBoundB;

  Beam(const Vector mBoundA, const Vector mBoundB)
      : mBoundA(mBoundA), mBoundB(mBoundB) {}
};

////////////////////////////////////////////////////////////
/// Represents a beam with bounds that extend to infinity.
struct UnboundBeam {
  Ray bound_a_, bound_b_;
    UnboundBeam(): UnboundBeam({},{}) {}
  UnboundBeam(Ray bound_a, Ray bound_b)
      : bound_a_(bound_a), bound_b_(bound_b) {}
};

    bool Contains(rt::UnboundBeam u_beam, rt::LineSegment line_segment);

    rt::Point Origin(rt::UnboundBeam u_beam);

} // namespace rt

std::ostream &operator<<(std::ostream &os, const rt::UnboundBeam &u_beam);

std::ostream &operator<<(std::ostream &os, const rt::Beam &beam);

bool operator==(const rt::Beam &first, const rt::Beam &second);

bool operator!=(const rt::Beam &first, const rt::Beam &second);

bool operator==(const rt::UnboundBeam &first, const rt::UnboundBeam &second);

bool operator!=(const rt::UnboundBeam &first, const rt::UnboundBeam &second);

#endif // RT_BEAM_H