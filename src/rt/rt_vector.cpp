#include "rt_vector.hpp"

using namespace std;
using namespace rt;

Vector operator*(float scalar, Vector vec) {
  auto newDest =
      vec.origin + (vec.dest - vec.origin) * scalar;
  return {vec.origin, newDest};
}

ostream& operator<<(ostream& os, const Vector& vec) {
  return os << "Vector(origin=" << vec.origin
            << ", dest=" << vec.dest << ')';
}

bool operator==(const Vector& first, const Vector& second) {
  return first.origin == second.origin &&
         first.dest == second.dest;
}

bool operator!=(const Vector& first, const Vector& second) {
  return !(first == second);
}