#ifndef RT_LINALG_HPP
#define RT_LINALG_HPP

namespace rt {
namespace linalg {

// Standard 2x2 matrix
// | a b |
// | c d |
struct Matrix2x2 {
  float a, b, c, d;

  Matrix2x2(float a, float b, float c, float d)
      : a(a), b(b), c(c), d(d) {}

  // Standard matrix determinant, (a * d) - (b * c)
  float determinant() const;
};

}  // namespace linalg
}  // namespace rt

#endif  // RT_LINALG_HPP