#ifndef rt_beam_h
#define rt_beam_h

#include "rt_vector.hpp"

namespace rt {

    struct Beam {
        const Vector mBoundA, mBoundB;

        Beam(Vector mBoundA, Vector mBoundB) :
                mBoundA(mBoundA), mBoundB(mBoundB) {}
    };
}

#endif // rt_beam_h
