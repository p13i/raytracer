//
//  rt_vector.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_vector.hpp"

rt::Vector operator * (int scalar, rt::Vector vec) {
    auto newDest = vec.origin + (vec.dest - vec.origin) * scalar;
    return rt::Vector(vec.origin, newDest);
}

rt::Vector operator * (float scalar, rt::Vector vec) {
    return vec * scalar;
}

std::ostream& operator << (std::ostream& os, const rt::Vector& vec)
{
    os << "Vector(";
    os << vec.origin << ", " << vec.dest << ')';
    return os;
}
