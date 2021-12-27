#include "rt_math.hpp"

using namespace std;

bool rt::math::inrange(float a, float lower, float upper) {
    return lower <= a && a <= upper;
}
