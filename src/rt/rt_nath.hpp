#ifndef rt_random_h
#define rt_random_h

#include <math/ofMathConstants.h>

#include <cmath>

namespace rt {
namespace random {
float sinusodal(float freqencyHz, float phase) {
  return sin(freqencyHz * 2 * PI * phase);
};
}  // namespace random
}  // namespace rt

#endif  // rt_random_h