#ifndef RT_GEO_HPP
#define RT_GEO_HPP

#include <vector>

#include "rt_linesegment.hpp"
#include "rt_point.hpp"
#include "rt_ray.hpp"
#include "rt_vector.hpp"

namespace rt {

using Polygon = std::vector<Point>;

namespace geo {

// Performs a standard dot product.
float dot(const Point& a, const Point& b);

// Indicates if the two line segments given intersect. If
// the lines intersect solely on an endpoint, it is
// considered an intersection. If the return value is true,
// the intersectionPoint passed in will have a value.
bool intersection(LineSegment j, LineSegment k,
                  Point& intersectionPoint);

// Reflects the given vector about the intersected edge as
// in this drawing:
//	forwardTrace -> \    / <- return value
//	                 \  /
//				      \/
//                 ------- <- intersectedEdge
Ray reflect(Vector forwardTrace,
            LineSegment intersectedEdge);

// Standard normal direction vector against a line segment.
Vector normal(LineSegment ls);

// The Euclidean, straight line distance between two points.
float dist(Point a, Point b);

// Rotates a ray by a given offset (in radians), clockwise
// following the positive direction on a unit circle.
Ray rotate(Ray r, float offsetRadians);

/// Finds the intersection point of two rays.
bool intersection_of_rays(Ray ray1, Ray ray2,
                          Point& intersection_point);

/// Determines if a point is between two rays
bool point_between_rays(const Ray& a, const Ray& b,
                        const Point& p);

}  // namespace geo
}  // namespace rt

#endif  // RT_GEO_HPP