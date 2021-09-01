//
//  rt_hyperparams.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#ifndef rt_hyperparams_hpp
#define rt_hyperparams_hpp

#include "rt_math.hpp"

#define RT_RAY_MARCH_TIME_STEP (2 << 0)
#define RT_RAY_MARCH_TIME_LIMIT (2 << 10)
#define RT_MAX_DEPTH (2 << 2)
#define RT_ENVIRONMENT_FILE "/Users/p13i/Desktop/raytracer/raytracer/data/environment.txt"
#define RT_SPREAD_RADIANS (RT_PI / 4.f)
#define RT_NUM_RAYS (2 << 6)

#endif /* rt_hyperparams_hpp */
