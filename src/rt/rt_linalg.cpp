#include "rt_linalg.hpp"

float rt::linalg::Matrix2x2::determinant() const {
  return a * d - c * b;
}