#include "rt_math.hpp"

#include <cmath>

float rt::math::sinusodal(float freqencyHz, float phase) {
    return sin(freqencyHz * 2 * RT_PI * phase);
};

bool rt::math::inrange(float a, float lower, float upper) {
    return lower <= a && a <= upper;
}
