#ifndef RT_GEOMETRY_HPP
#define RT_GEOMETRY_HPP

#include <vector>

#include "rt_linesegment.hpp"
#include "rt_ray.hpp"

namespace rt {

// Contains the discrete geometry for a scene. Currently,
// only consists of line segments.
struct Geometry {
  // Walls
  std::vector<LineSegment> edges;

  Geometry() : Geometry(std::vector<LineSegment>()) {}

  explicit Geometry(std::vector<LineSegment> edges)
      : edges(std::move(edges)) {}

  bool intersection(Ray start, Point &intersectionPoint,
                    LineSegment &intersectedEdge) const;
};

}  // namespace rt

#endif  // RT_GEOMETRY_HPP