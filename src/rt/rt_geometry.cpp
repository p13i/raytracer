#include "rt_geometry.hpp"
#include "rt_ray.hpp"
#include "rt_point.hpp"
#include "rt_settings.hpp"
#include "rt_geo.hpp"

bool rt::Geometry::intersection(Ray start, Point& intersectionPoint, LineSegment& intersectedEdge) {
    float time = 0;

    bool intersection = false;
    while (!intersection) {

        Point marchedRayPoint = start(time);

        // Find first intersection via ray marching
        for (rt::LineSegment edge : this->edges) {

            if (rt::geo::intersection(edge, {start.origin, marchedRayPoint}, intersectionPoint)) {

                float rayOriginToIxnPointDist = rt::geo::dist(start.origin, intersectionPoint);
                float marchedPtToIxnPointDistance = rt::geo::dist(marchedRayPoint, intersectionPoint);

                // Make sure the ray was facing to the intersection point
                if (rayOriginToIxnPointDist > marchedPtToIxnPointDistance) {
                    intersection = true;
                    intersectedEdge = edge;
                    break;
                }
            }
        }

        if (!intersection) {
            time += RT_RAY_MARCH_TIME_STEP;
        }

        if (time > RT_RAY_MARCH_TIME_LIMIT) {
            break;
        }
    }

    return intersection;
}
