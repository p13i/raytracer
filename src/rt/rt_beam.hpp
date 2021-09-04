//
//  rt_beam.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 9/1/21.
//

#ifndef rt_beam_h
#define rt_beam_h

#include "rt_vector.hpp"

namespace rt {
class Beam {
    
public:
    Vector mBoundA, mBoundB;
    Beam(Vector mBoundA, Vector mBoundB): mBoundA(mBoundA), mBoundB(mBoundB) { }
};
}

#endif /* rt_beam_h */
