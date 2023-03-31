#include "rt_math.hpp"

#include <cmath>

float rt::math::sinusodal(float freqencyHz, float phase) {
  return static_cast<float>(
      sin(freqencyHz * 2.f * RT_PI * phase));
};

bool rt::math::inrange(float a, float lower, float upper) {
  return lower <= a && a <= upper;
}