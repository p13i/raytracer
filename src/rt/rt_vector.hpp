#ifndef RT_VECTOR_HPP
#define RT_VECTOR_HPP

#include <ostream>

#include "rt_linesegment.hpp"
#include "rt_point.hpp"

namespace rt {

struct Vector {
  Point origin, dest;  // from, to

  Vector(Point dest) : origin({0, 0}), dest(dest) {}

  Vector(Point orig, Point dest)
      : origin(orig), dest(dest) {}

  Point _direction() const { return dest - origin; }

  Vector unit() const {
    return {_direction() / magnitude()};
  }

  LineSegment lineSegment() const { return {origin, dest}; }

  float magnitude() const {
    auto dir = _direction();
    return sqrtf(dir.x * dir.x + dir.y * dir.y);
  }

  Vector operator*(float scalar) const {
    auto newDest = origin + (dest - origin) * scalar;
    return {origin, newDest};
  }

  Vector operator-(Vector other) const {
    return {origin - other.origin, dest - other.dest};
  }
};

}  // namespace rt

rt::Vector operator*(int scalar, rt::Vector vec);

rt::Vector operator*(float scalar, rt::Vector vec);

std::ostream& operator<<(std::ostream& os,
                         const rt::Vector& vec);

bool operator==(const rt::Vector& first,
                const rt::Vector& second);

bool operator!=(const rt::Vector& first,
                const rt::Vector& second);

#endif  // RT_VECTOR_HPP