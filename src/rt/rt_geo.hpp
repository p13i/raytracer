//
//  rt_geo.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_geo_hpp
#define rt_geo_hpp

#include <stdio.h>

#include "rt_point.hpp"
#include "rt_linesegment.hpp"
#include "rt_point.hpp"
#include "rt_vector.hpp"
#include "rt_ray.hpp"


namespace rt { namespace geo {

float dot(const rt::Point& a, const rt::Point& b);

// computes the intersection point, indicating if there was one
bool intersection(LineSegment j, LineSegment k, Point& intersectionPoint);

rt::Ray reflect(rt::Vector forwardTrace, LineSegment intersectedEdge);

rt::Vector normal(rt::LineSegment ls);

float dist(Point a, Point b);

rt::Ray rotate(rt::Ray r, float offsetRadians);

} }

#endif /* rt_geo_hpp */
