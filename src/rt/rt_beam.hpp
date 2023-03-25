#ifndef RT_BEAM_H
#define RT_BEAM_H

#include "rt_vector.hpp"

namespace rt {

////////////////////////////////////////////////////////////
/// Represents a beam with finite bounds.
struct Beam {
  Vector mBoundA, mBoundB;

  Beam(const Vector mBoundA, const Vector mBoundB)
      : mBoundA(mBoundA), mBoundB(mBoundB) {}
};

}  // namespace rt

std::ostream &operator<<(std::ostream &os, const rt::Beam &beam);

bool operator==(const rt::Beam &first, const rt::Beam &second);

bool operator!=(const rt::Beam &first, const rt::Beam &second);

#endif  // RT_BEAM_H