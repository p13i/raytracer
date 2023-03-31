#ifndef RT_RANDOM_HPP
#define RT_RANDOM_HPP

#include <cmath>

#include "rt_math.hpp"

namespace rt {
namespace random {
// Standard sine function. Phase should be in [0,1].
// Frequency should be positive.
float sinusodal(float frequencyHz, float phase) {
  return sin(frequencyHz * 2 * RT_PI * phase);
};
}  // namespace random
}  // namespace rt

#endif  // RT_RANDOM_HPP