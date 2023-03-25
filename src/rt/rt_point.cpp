#include "rt_point.hpp"

#include <cmath>

#include "rt_settings.hpp"

using namespace rt;
using namespace std;

bool Point::operator==(Point other) const {
  float xDiff = abs(x - other.x);
  float yDiff = abs(y - other.y);
  return xDiff < RT_EPSILON && yDiff < RT_EPSILON;
}

bool Point::operator!=(Point other) const { return !(*this == other); }

Point Point::operator+(Point other) const { return {x + other.x, y + other.y}; }

Point Point::operator-(Point other) const { return {x - other.x, y - other.y}; }

Point Point::operator*(float scalar) const { return {x * scalar, y * scalar}; }

Point Point::operator/(float scalar) const { return {x / scalar, y / scalar}; }

// Source: https://stackoverflow.com/a/41648840
// Required for use in std::set
bool Point::operator<(Point other) const {
  return (x < other.x) || (other.x >= x && (y < other.y));
}

ostream &operator<<(ostream &os, const Point &point) {
  return os << "Point(" << point.x << ", " << point.y << ')';
}