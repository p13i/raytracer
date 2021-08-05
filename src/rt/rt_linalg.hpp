//
//  rt_linalg.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_linalg_hpp
#define rt_linalg_hpp

#include "rt_vector.hpp"


namespace rt { namespace linalg {

// | a b |
// | c d |
struct Matrix2x2 {
    float a, b, c, d;
    
    Matrix2x2(float a, float b, float c, float d): a(a), b(b), c(c), d(d) { }
    
    float determinant();
};

float angleBetween(rt::Vector a, rt::Vector b);

} }

#endif /* rt_linalg_hpp */
