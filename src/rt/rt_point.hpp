#ifndef rt_point_hpp
#define rt_point_hpp

#include <ostream>

#include "rt_settings.hpp"

namespace rt {

struct Point {
  float x, y;

  Point() : Point(0, 0) {}

  Point(float x, float y) : x(x), y(y) {}

  bool operator==(Point other) const;

  bool operator!=(Point other) const;

  Point operator+(Point other) const;

  Point operator-(Point other) const;

  Point operator*(float scalar) const;

  Point operator/(float scalar) const;

  bool operator<(Point other) const;
};

}  // namespace rt

std::ostream &operator<<(std::ostream &os, const rt::Point &point);

#endif /* rt_point_hpp */