#ifndef rt_geometry_hpp
#define rt_geometry_hpp

#include <vector>

#include "rt_linesegment.hpp"
#include "rt_ray.hpp"

namespace rt {

struct Geometry {
  std::vector<LineSegment> edges;

  Geometry() : Geometry(std::vector<LineSegment>()) {}

  Geometry(std::vector<LineSegment> edges) : edges(edges) {}

  bool intersection(Ray start, Point &intersectionPoint,
                    LineSegment &intersectedEdge);
};

}  // namespace rt

#endif /* rt_geometry_hpp */