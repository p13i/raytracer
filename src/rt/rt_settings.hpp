#ifndef rt_settings_hpp
#define rt_settings_hpp

#include "rt_math.hpp"

#define RT_EPSILON              0.01
#define RT_RAY_MARCH_TIME_STEP  (2 << 0)
#define RT_RAY_MARCH_TIME_LIMIT (2 << 10)
#define RT_MAX_DEPTH            (2 << 2)
#define RT_SPREAD_RADIANS       (RT_PI / 4.f)
#define RT_NUM_RAYS             (2 << 3)

#endif // rt_settings_hpp
