#ifndef rt_geo_hpp
#define rt_geo_hpp

#include "rt_point.hpp"
#include "rt_linesegment.hpp"
#include "rt_point.hpp"
#include "rt_vector.hpp"
#include "rt_ray.hpp"

#include <cstdio>
#include <vector>

namespace rt {
    
using Polygon = std::vector<Point>;
    
namespace geo {

    
////////////////////////////////////////////////////////////
// Performs a standard dot product.
float dot(const Point& a, const Point& b);

////////////////////////////////////////////////////////////
// Indicates if the two line segments given intersect. If
// the lines intersect solely on an endpoint, it is
// considered an intersection. If the return value is true,
// the intersectionPoint passed in will have a value.
bool intersection(LineSegment j, LineSegment k, Point& intersectionPoint);

////////////////////////////////////////////////////////////
// Reflects the given vector about the intersected edge as
// in this drawing:
//	forwardTrace -> \    / <- return value
//	                 \  /
//				      \/
//                 ------- <- intersectedEdge
Ray reflect(Vector forwardTrace, LineSegment intersectedEdge);

////////////////////////////////////////////////////////////
// Standard normal direction vector against a line segment.
Vector normal(LineSegment ls);

////////////////////////////////////////////////////////////
// The Euclidean, straight line distance between two points.
float dist(Point a, Point b);

////////////////////////////////////////////////////////////
// Rotates a ray by a given offset (in radians), clockwise
// following the positive direction on a unit circle.
Ray rotate(Ray r, float offsetRadians);

    struct gptRay {
        Point origin;
        Point direction;
    };
    bool gpt_intersects(const geo::gptRay &ray1, const geo::gptRay &ray2,
                    Point &intersection);
////////////////////////////////////////////////////////////
/// Finds the intersection point of two rays.
bool intersection_of_rays(Ray a, Ray b, Point& intersection_point);

}   // namespace geo
}   // namespace rt

#endif // rt_geo_hpp
