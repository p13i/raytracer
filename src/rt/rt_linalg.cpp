//
//  rt_linalg.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_linalg.hpp"

#include "rt_settings.hpp"

float rt::linalg::Matrix2x2::determinant() const {
    return a * d - c * b;
}
