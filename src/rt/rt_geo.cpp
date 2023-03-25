//
//  rt_geo.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_geo.hpp"

#include <cmath>

#include "rt_beam.hpp"
#include "rt_linalg.hpp"
#include "rt_ray.hpp"
#include "rt_settings.hpp"
#include "rt_unbound_beam.hpp"
#include "rt_vector.hpp"

using namespace std;
using namespace rt;

bool rt::geo::intersection(LineSegment j, LineSegment k, Point& intersectionPoint) {
    // First check if the line segments intersect at the endpoints
    if (j.a == k.a || j.a == k.b) {
        intersectionPoint = j.a;
        return true;
    } else if (j.b == k.a || j.b == k.b) {
        intersectionPoint = j.b;
        return true;
    }
    
    // Run the intersection algorithm
    float x1 = j.a.x, y1 = j.a.y;
    float x2 = j.b.x, y2 = j.b.y;
    float x3 = k.a.x, y3 = k.a.y;
    float x4 = k.b.x, y4 = k.b.y;
    
    float denominator = rt::linalg::Matrix2x2(
        x1 - x2, x3 - x4,
        y1 - y2, y3 - y4).determinant();
    
    if (denominator == 0.f) {
        return false;
    }
    
    float t = rt::linalg::Matrix2x2(
        x1 - x3, x3 - x4,
        y1 - y3, y3 - y4).determinant();
    
    t /= denominator;
    
    float u = rt::linalg::Matrix2x2(
        x2 - x1, x1 - x3,
        y2 - y1, y1 - y3).determinant();
    
    u /= denominator;
    
    if (rt::math::inrange(t) && rt::math::inrange(u)) {
        float px = x1 + t * (x2 - x1), py = y1 + t * (y2 - y1);
        intersectionPoint = rt::Point(px, py);
        return true;
    }
    
    return false;
}

float rt::geo::dist(Point a, Point b) {
    return Vector(a, b).magnitude();
}

rt::Vector rt::geo::normal(rt::LineSegment ls) {
    float x1 = ls.a.x, y1 = ls.a.y, x2 = ls.b.x, y2 = ls.b.y;
    float dx = x2 - x1, dy = y2 - y1;
    return Vector(Point{-dy, dx}).unit();
}

float rt::geo::dot(const rt::Point& a, const rt::Point& b) {
    return a.x * b.x + a.y * b.y;
}

Ray geo::reflect(Vector forwardTrace, LineSegment intersectedEdge) {
    
    Point intersectionPoint;
    if (!geo::intersection(forwardTrace.lineSegment(), {intersectedEdge.a, intersectedEdge.b}, intersectionPoint)) {
        return Ray{};
    }

    auto d = forwardTrace.unit();
    auto n = rt::geo::normal(intersectedEdge);
    auto dot = rt::geo::dot(forwardTrace.unit().dest, n.unit().dest);
    auto r = d - 2.f * n * dot;
    r = r.unit();
    
    return rt::Ray(forwardTrace.dest, r);
}

rt::Ray rt::geo::rotate(rt::Ray r, float offsetRadians) {
    return Ray(r.origin, r.radians() + offsetRadians);
}

bool rt::geo::intersection_of_rays(Ray ray1, Ray ray2, Point& intersection_point) {
    // TODO remove hack, use Cramer's method
    LineSegment line1{ray1(0), ray1(1000)};
    LineSegment line2{ray2(0), ray2(1000)};
    return intersection(line1, line2, intersection_point);
}

bool rt::geo::point_between_rays(const Ray& a, const Ray& b, const Point& p) {
    // Assign the left and right variables to the
    // rays' orientations on a unit circle. A larger
    // radians theta means the ray is more to the left,
    // or counter-clockwise on the unit circle.
    Ray left;
    Ray right;
    if (a.radians() < b.radians()) {
        left = a;
        right = b;
    } else {
        left = b;
        right = a;
    }

    // Now compute the radians to the point to see if
    // it is in the bounds of the left and right radians
    // First, get the shared origin of the rays.
    Point origin = Origin({left, right});
    float rad_to_p = Ray(origin, p).radians();

    return left.radians() <= rad_to_p && rad_to_p <= right.radians();
}
