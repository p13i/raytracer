//
//  raytracer.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/29/21.
//

#include "raytracer.hpp"

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>

// *****
// operator implementations
// *****

std::ostream& operator << (std::ostream& os, const rt::Point& point)
{
    os << "Point(" << point.x << ", " << point.y << ')';
    return os;
}

std::ostream& operator << (std::ostream& os, const rt::LineSegment& edge)
{
    os << "LineSegment(";
    os << edge.a;
    os << ", " << edge.b << ')';
    return os;
}

std::ostream& operator << (std::ostream& os, const rt::Vector& vec)
{
    os << "Vector(";
    os << vec.origin << ", " << vec.dest << ')';
    return os;
}


bool rt::math::inrange(float a, float lower, float upper) {
    return lower <= a && a <= upper;
}


bool rt::Point::operator == (rt::Point other) {
    return rt::math::fequal(x, other.x) && rt::math::fequal(y, other.y);
}

// https://stackoverflow.com/a/32983646/5071723
int printTimestamp() {
    const int bufferSize = 29;
  char buffer[bufferSize];
  int millisec;
  struct tm* tm_info;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
  if (millisec>=1000) { // Allow for rounding up to nearest second
    millisec -=1000;
    tv.tv_sec++;
  }

  tm_info = localtime(&tv.tv_sec);

  strftime(buffer, bufferSize, "[%Y:%m:%d %H:%M:%S", tm_info);
  printf("%s.%03d] ", buffer, millisec);

  return 0;
}

void rt::log(const char *str, bool timestamp, bool newline) {
    if (timestamp) {
        printTimestamp();
    }
    
    std::cout << str;
    
    if (newline) {
        std::cout << std::endl;
    }
}

void rt::log(std::string str, bool timestamp, bool newline) {
    rt::log(str.c_str(), timestamp, newline);
}

void printm(const char *str) {
    std::cout << str;
}

void tests() {
    
    RT_TEST("rt::geo::intersection(LineSegment, LineSegment, Point&) - trivial", {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{0, 0}, {0, 1}}, {{0, 1}, {1, 1}}, intersectionPoint);
        
        RT_ASSERT(doIntersect, "Lines should intersect");
        RT_ASSERT(intersectionPoint == rt::Point(0, 1), "Intersection should be at (0, 1)");
    });
    
    RT_TEST("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting at origin", {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-1, +1}, {+1, -1}}, intersectionPoint);
        RT_ASSERT(doIntersect, "Lines should intersect");

        RT_ASSERT(intersectionPoint == rt::Point(0, 0), "Intersection should be at (0, 0)");
    });
    
    RT_TEST("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting in Q3", {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-0.5, +0.5}, {+1, 0.5}}, intersectionPoint);
        RT_ASSERT(doIntersect, "Lines should intersect");

        RT_ASSERT(intersectionPoint == rt::Point(0.5, 0.5), "Intersection should be at (0.5, 0.5)");
    });
    
    RT_TEST("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting in Q3", {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-0.5, +0.5}, {+1, 0.5}}, intersectionPoint);
        RT_ASSERT(doIntersect, "Lines should intersect");

        RT_ASSERT(intersectionPoint == rt::Point(0.5, 0.5), "Intersection should be at (0.5, 0.5)");
    });
    
    RT_TEST("rt::geo::reflect(rt::Vector, rt::LineSegment)", {
        
        /*
        (-1, 1) - reflected ray, normalized=(-sqrt(2)/2, sqrt(2)/2)
                  (1, 1) - incident ray (forward trace), normalized=(sqrt(2)/2, sqrt(2)/2)
         
         \        /
                 /
           \    /
               /
             \/  <- theta = -0.7071 = sqrt(2) / 2
         ---------- <- y = 0
         ^   ^^   ^
         -1 orig  1
         */
        
        rt::Vector forwardTrace({1, 1}, {0, 0});
        rt::LineSegment intersectedEdge({-1, 0}, {1, 0});
        
        rt::Ray reflection = rt::geo::reflect(forwardTrace, intersectedEdge);
        
        RT_ASSERT(RT_FEQUAL(reflection.direction.dest.x, -1 * sqrt(2) / 2.f), "x should be -sqrt(2)/2.f");
        RT_ASSERT(RT_FEQUAL(reflection.direction.dest.y, sqrt(2) / 2.f), "y should be sqrt(2) / 2.f");
    });
    
    RT_TEST("RayTracer::trace(Environment, Point) - sanity case", {
        
        // Set up raytracer with environment
        
        rt::Environment env{
            /* geometry: */ {
                /* edges: */ std::vector<rt::LineSegment>({
                    rt::LineSegment({-1, +1}, {+1, +1}),
                    rt::LineSegment({+1, +1}, {+1, -1}),
                    rt::LineSegment({+1, -1}, {-1, -1}),
                    rt::LineSegment({-1, -1}, {-1, +1}),
                })
            }
        };
        
        rt::RayTracer raytracer{env};
        
        {
            rt::Ray startingRay({0, 0}, {-1, -1});

            // Trace rays
            std::vector<rt::Vector> paths = raytracer.trace(startingRay, 1);

            RT_ASSERT(paths.size() == 1, "There should be one path");
            RT_ASSERT(paths[0].origin == rt::Point(0.f, 0.f), "This^ path should have with origin (0, 0)");
            RT_ASSERT(paths[0].dest == rt::Point(-1.f, -1.f), "This^ path should have ray dest at (-1, -1)");
        }

        {
            rt::Ray startingRay({0, 0}, {0.5, 0.25});

            // Trace rays
            std::vector<rt::Vector> paths = raytracer.trace(startingRay, 1);

            RT_ASSERT(paths.size() == 1, "There should be one path");
            RT_ASSERT(paths[0].origin == rt::Point(0.f, 0.f), "This^ path should have with origin (0, 0)");
            RT_ASSERT(paths[0].dest == rt::Point(1.f, 0.5f), "This^ path should have ray dest at (1, 0.5)");
        }
        
        {
            rt::Ray startingRay({0, 0}, {0.5, 0.25});
            
            // Trace rays
            std::vector<rt::Vector> paths = raytracer.trace(startingRay, 3);
            
            PRINTLN(paths.size());
            PRINTLN(paths[0]);
            PRINTLN(paths[1]);
            PRINTLN(paths[2]);
            
            
            
            RT_ASSERT(paths.size() == 3, "There should be one path");
            
            RT_ASSERT(paths[0].origin == rt::Point(0.f, 0.f), "This path should have with origin (0, 0)");
            RT_ASSERT(paths[0].dest == rt::Point(1.f, 0.5f), "This path should have ray dest at (1, 0.5)");
            RT_ASSERT(paths[0].dest == paths[1].origin, "d of first ray is o of second ray");
            RT_ASSERT(paths[1].dest == rt::Point(0.f, 1.f), "d of second ray is (0, 1)");
            RT_ASSERT(paths[1].dest == paths[2].origin, "d of first day is o of second ray");
            RT_ASSERT(paths[2].dest == rt::Point(-1, 0.5f), "d of second day is (-1, 0)");
        }
    });
}

int rt::main() {
    rt:log("rt:main()");
    
    tests();
}

std::vector<rt::Vector> rt::RayTracer::trace(rt::Ray start, unsigned int depth) {
    if (depth == 0) {
        return {};
    }
    
    std::vector<rt::Vector> tracedPaths;
    
    float time = 0.;
    
    bool intersection = false;
    Point intersectionPoint;
    rt::LineSegment intersectedEdge;
    while (!intersection) {
        
        Point marchedRayPoint = start(time);
    
        // Find first intersection via ray marching
        for (rt::LineSegment edge : this->environment.geometry.edges) {
            
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
    
    if (intersection) {
        rt::Vector forwardTrace = rt::Vector(start.origin, intersectionPoint);
        tracedPaths.push_back(forwardTrace);
        
        // recurse on the reflected ray
        
        // reflect forwardTrace vector over intersectedEdge
        rt::Ray reflectedRay = rt::geo::reflect(forwardTrace, intersectedEdge);
        
        auto subTraces = this->trace(reflectedRay, depth - 1);
        
        for (auto trace : subTraces) {
            tracedPaths.push_back(trace);
        }
    }
    
    return tracedPaths;
}

rt::Vector rt::geo::normal(rt::LineSegment ls) {
    float x1 = ls.a.x, y1 = ls.a.y, x2 = ls.b.x, y2 = ls.b.y;
    float dx = x2 - x1, dy = y2 - y1;
    return Vector({-dy, dx}).unit();
}

float rt::geo::dot(const rt::Point& a, const rt::Point& b) {
    return a.x * b.x + a.y * b.y;
}

float rt::math::clamp(float value, float lower, float upper) {
    if (value > upper) {
        return upper;
    } else if (value < lower) {
        return lower;
    }
    return value;
}


bool rt::math::fequal(float a, float b, float threshold) {
    return abs(a - b) <= threshold;
}

float rt::linalg::Matrix2x2::determinant() {
    return a * d - c * b;
}

bool rt::geo::intersection(LineSegment j, LineSegment k, OUT Point& intersectionPoint) {
    float x1 = j.a.x, y1 = j.a.y;
    float x2 = j.b.x, y2 = j.b.y;
    float x3 = k.a.x, y3 = k.a.y;
    float x4 = k.b.x, y4 = k.b.y;
    
    float denominator = rt::linalg::Matrix2x2(
        x1 - x2, x3 - x4,
        y1 - y2, y3 - y4).determinant();
    
    if (RT_FEQUAL(denominator, 0.f)) {
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

rt::Vector operator * (int scalar, rt::Vector vec) {
    auto newDest = vec.origin + (vec.dest - vec.origin) * scalar;
    return rt::Vector(vec.origin, newDest);
}

rt::Ray rt::geo::reflect(rt::Vector forwardTrace, LineSegment intersectedEdge) {
    
    rt::Point intersectionPoint;
    if (!rt::geo::intersection(forwardTrace.lineSegment(), {intersectedEdge.a, intersectedEdge.b}, intersectionPoint)) {
        return;
    }
    
//    std::cout << intersectionPoint << std::endl;
    
//    float theta = rt::linalg::angleBetween(forwardTrace, {intersectedEdge.a, intersectedEdge.b});
//
//    std::cout << theta << std::endl;
    
    auto d = forwardTrace.unit();
    auto n = rt::geo::normal(intersectedEdge);
    auto dot = rt::geo::dot(forwardTrace.unit().dest, rt::geo::normal(intersectedEdge).unit().dest);
    auto r = d - 2 * n * dot;
    r = r.unit();
    
    return rt::Ray(forwardTrace.dest, r);
}

float rt::linalg::angleBetween(rt::Vector u, rt::Vector v) {
    rt::Point pu = u.unit().dest, pv = v.unit().dest;
    return rt::geo::dot(pu, pv) / (pu.magnitude() * pv.magnitude());
}

rt::Vector operator * (float scalar, rt::Vector vec) {
    return vec * scalar;
}

float rt::geo::dist(Point a, Point b) {
    return Vector(a, b).magnitude();
}
