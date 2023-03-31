#include "rt_ray.hpp"

#include "rt_math.hpp"

using namespace rt;

float Ray::radians() const {
  auto dir = direction.unit().dest;
  float theta = atanf(dir.y / dir.x);

  // Source: https://math.stackexchange.com/a/1183407/414919
  if (dir.x < 0) {
    theta += RT_PI;
  }

  return theta;
}

Point Ray::operator()(float time) const {
  return origin + (direction * time).dest;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
  return os << "Ray(origin=" << ray.origin
            << ", direction=" << ray.direction << ")";
}

bool operator==(const Ray& first, const Ray& second) {
  return first.origin == second.origin &&
         first.direction == second.direction;
}

bool operator!=(const Ray& first, const Ray& second) {
  return !(first == second);
}