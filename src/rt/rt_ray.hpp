#ifndef RT_RAY_HPP
#define RT_RAY_HPP

#include "rt_point.hpp"
#include "rt_vector.hpp"

namespace rt {

// A unbounded vector
struct Ray {
  Point origin;
  Vector direction;

  // Empty constructor
  Ray() : Ray({}, {}) {}

  // The direction in radians on the Unit Circle from a point
  Ray(const Point origin, const float radians)
      : Ray(origin, Vector{{cosf(radians), sinf(radians)}}) {}

  // A
  Ray(const Point origin, const Point towards)
      : Ray(origin, Vector(towards - origin).unit()) {}

  Ray(const Point origin, const Vector direction)
      : origin(origin), direction(direction.unit()) {}

  Ray(const Vector vector) : Ray(vector.origin, vector.dest) {}

  Point operator()(float time) const;

  float radians() const;
};

}  // namespace rt

std::ostream &operator<<(std::ostream &os, const rt::Ray &ray);

bool operator==(const rt::Ray &first, const rt::Ray &second);

bool operator!=(const rt::Ray &first, const rt::Ray &second);

#endif  // RT_RAY_HPP