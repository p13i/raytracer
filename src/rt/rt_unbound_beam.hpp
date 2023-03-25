#ifndef RT_UNBOUND_BEAM_HPP
#define RT_UNBOUND_BEAM_HPP

#include <ostream>

#include "rt_point.hpp"
#include "rt_ray.hpp"

namespace rt {

/// Represents a beam with bounds that extend to infinity.
struct UnboundBeam {
  Ray bound_a_{}, bound_b_{};
  UnboundBeam() : UnboundBeam({}, {}) {}
  UnboundBeam(const Ray bound_a, const Ray bound_b)
      : bound_a_(bound_a), bound_b_(bound_b) {}
};

bool Contains(UnboundBeam u_beam, LineSegment line_segment);

Point Origin(UnboundBeam u_beam);

}  // namespace rt

std::ostream &operator<<(std::ostream &os, const rt::UnboundBeam &u_beam);

bool operator==(const rt::UnboundBeam &first, const rt::UnboundBeam &second);

bool operator!=(const rt::UnboundBeam &first, const rt::UnboundBeam &second);

#endif  // RT_UNBOUND_BEAM_HPP