//
//  rt_linalg.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_linalg_hpp
#define rt_linalg_hpp

#include "rt_vector.hpp"
#include "rt_settings.hpp"


namespace rt { namespace linalg {

// | a b |
// | c d |
struct Matrix2x2 {
    float a, b, c, d;
    
    Matrix2x2(float a, float b, float c, float d): a(a), b(b), c(c), d(d) { }

    float determinant() const;
};

} }

#endif /* rt_linalg_hpp */
