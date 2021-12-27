#ifndef rt_geo_hpp
#define rt_geo_hpp

#include "rt_point.hpp"
#include "rt_linesegment.hpp"
#include "rt_point.hpp"
#include "rt_vector.hpp"
#include "rt_ray.hpp"

#include <cstdio>
#include <vector>

using namespace std;

namespace rt {
    namespace geo {

        float dot(const Point &a, const Point &b);

        bool intersection(LineSegment j, LineSegment k, Point &intersectionPoint);

        Ray reflect(Vector forwardTrace, LineSegment intersectedEdge);

        Vector normal(LineSegment ls);

        float dist(Point a, Point b);

        Ray rotate(Ray r, float offsetRadians);

    }
}

#endif // rt_geo_hpp
