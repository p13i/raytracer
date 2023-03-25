#ifndef rt_math_hpp
#define rt_math_hpp

// Source: https://stackoverflow.com/a/507879
#define RT_PI \
  3.1415926535897932384626433832795028841971693993751058209749445923078164062

namespace rt {

namespace math {

float sinusodal(float freqencyHz, float phase);

bool inrange(float a, float lower = 0., float upper = 1.);

};  // namespace math

}  // namespace rt

#endif  // rt_math_hpp