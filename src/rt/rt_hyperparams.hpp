//
//  rt_hyperparams.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#ifndef rt_hyperparams_hpp
#define rt_hyperparams_hpp

#include "rt_math.hpp"

//#define DRAW_TRACE
#define DRAW_CAST

#define RT_RAY_MARCH_TIME_STEP 0.01
#define RT_RAY_MARCH_TIME_LIMIT 1000
#define RT_MAX_DEPTH 8
#define RT_ENVIRONMENT_FILE "C:\\Users\\p13i\\Projects\\raytracer\\data\\environment.txt"
#define RT_SPREAD_RADIANS (RT_PI / 4.f)
#define RT_NUM_RAYS 256
#define RT_EXTINGUISH_FLOAT 0.01

#endif /* rt_hyperparams_hpp */
