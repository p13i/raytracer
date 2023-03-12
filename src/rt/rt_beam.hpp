#ifndef rt_beam_h
#define rt_beam_h

#include "rt_vector.hpp"

namespace rt {

    struct Beam {
        const Vector mBoundA, mBoundB;

        Beam(Vector mBoundA, Vector mBoundB) :
                mBoundA(mBoundA), mBoundB(mBoundB) {}

        bool operator == (const Beam& other) {
            Vector thisA = this->mBoundA, otherA = other.mBoundA;
            Vector thisB = this->mBoundB, otherB = other.mBoundB;
            return thisA == otherA && thisB == otherB;
        }
    };
}

bool operator == (const rt::Beam& first, const rt::Beam& second);

bool operator != (const rt::Beam& first, const rt::Beam& second);

#endif // rt_beam_h
