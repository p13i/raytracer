//
//  rt_math.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_math_hpp
#define rt_math_hpp

#include <stdio.h>

namespace rt {

namespace math {

bool fequal(float a, float b, float threshold = 0.01);

float clamp(float value, float lower = 0., float upper = 1.);

bool inrange(float a, float lower = 0., float upper = 1.);

float sqrt(float f);

float abs(float f);

};

}

#endif /* rt_math_hpp */
