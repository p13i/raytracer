//
//  rt_vector.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_vector.hpp"

using namespace std;
using namespace rt;

Vector operator * (float scalar, Vector vec) {
    auto newDest = vec.origin + (vec.dest - vec.origin) * scalar;
    return {vec.origin, newDest};
}

ostream& operator << (ostream& os, const Vector& vec)
{
    return os << "Vector(" << vec.origin << ", " << vec.dest << ')';
}
