//
//  rt_linalg.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_linalg.hpp"

#include "rt_geo.hpp"

float rt::linalg::Matrix2x2::determinant() {
    return a * d - c * b;
}

float rt::linalg::angleBetween(rt::Vector u, rt::Vector v) {
    rt::Point pu = u.unit().dest, pv = v.unit().dest;
    return rt::geo::dot(pu, pv) / (pu.magnitude() * pv.magnitude());
}
