#ifndef rt_random_h
#define rt_random_h

#include <cmath>
#include <math/ofMathConstants.h>

namespace rt {
    namespace random {
        float sinusodal(float freqencyHz, float phase) {
            return sin(freqencyHz * 2 * PI * phase);
        };
    }
}

#endif // rt_random_h