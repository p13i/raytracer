#include "rt_beam.hpp"

bool operator == (const rt::Beam& first, const rt::Beam& second) {
    return (first.mBoundA == second.mBoundA) && (first.mBoundB == second.mBoundB);
}

bool operator != (const rt::Beam& first, const rt::Beam& second) {
    return !(first == second);
}
