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

  // The direction in radians on the Unit Circle from a
  // point
  Ray(const Point origin, const float radians)
      : Ray(origin,
            Vector{{cosf(radians), sinf(radians)}}) {}

  // A ray from an origin in the direction towards a point
  Ray(const Point origin, const Point towards)
      : Ray(origin, Vector(towards - origin).unit()) {}

  // A ray with a given origin and destination
  Ray(const Point origin, const Vector direction)
      : origin(origin), direction(direction.unit()) {}

  // Converting a vector directly to a ray where the
  // vector's origin is the origin of the ray
  explicit Ray(const Vector vector)
      : Ray(vector.origin, vector.dest) {}

  // Evaluates a ray at a given time-step multiplied against
  // the unit vector direction
  Point operator()(float time) const;

  // The radians orientation of the ray on the unit circle
  float radians() const;
};

}  // namespace rt

std::ostream &operator<<(std::ostream &os,
                         const rt::Ray &ray);

bool operator==(const rt::Ray &first,
                const rt::Ray &second);

bool operator!=(const rt::Ray &first,
                const rt::Ray &second);

#endif  // RT_RAY_HPP