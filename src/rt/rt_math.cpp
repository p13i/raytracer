//
//  rt_math.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_math.hpp"

#include <cmath>

float rt::math::sqrt(float f) {
    return std::sqrt(f);
}

bool rt::math::inrange(float a, float lower, float upper) {
    return lower <= a && a <= upper;
}

float rt::math::clamp(float value, float lower, float upper) {
    if (value > upper) {
        return upper;
    } else if (value < lower) {
        return lower;
    }
    return value;
}


float rt::math::abs(float a) {
    return std::abs(a);
}


bool rt::math::fequal(float a, float b, float threshold) {
    return rt::math::abs(a - b) <= threshold;
}
