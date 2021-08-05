//
//  rt_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_tests.hpp"

#include "rt_point.hpp"
#include "rt_geo.hpp"
#include "rt_log.hpp"
#include "rt_str.hpp"
#include "rt_raytracer.hpp"
#include "rt_ray.hpp"


void rt::test::run_tests() {
    
    
    Test("rt::geo::intersection(LineSegment, LineSegment, Point&) - trivial", []() {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{0, 0}, {0, 1}}, {{0, 1}, {1, 1}}, intersectionPoint);
        
        Assert(doIntersect, "Lines should intersect");
        Assert(intersectionPoint == rt::Point(0, 1), "Intersection should be at (0, 1)");
    });
    
    Test("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting at origin", []() {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-1, +1}, {+1, -1}}, intersectionPoint);
        Assert(doIntersect, "Lines should intersect");

        Assert(intersectionPoint == rt::Point(0, 0), "Intersection should be at (0, 0)");
    });
    
    Test("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting in Q3", []() {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-0.5, +0.5}, {+1, 0.5}}, intersectionPoint);
        Assert(doIntersect, "Lines should intersect");

        Assert(intersectionPoint == rt::Point(0.5, 0.5), "Intersection should be at (0.5, 0.5)");
    });
    
    Test("rt::geo::intersection(LineSegment, LineSegment, Point&) - intersecting in Q3", []() {
        rt::Point intersectionPoint;
        bool doIntersect = rt::geo::intersection({{-1, -1}, {+1, +1}}, {{-0.5, +0.5}, {+1, 0.5}}, intersectionPoint);
        Assert(doIntersect, "Lines should intersect");

        Assert(intersectionPoint == rt::Point(0.5, 0.5), "Intersection should be at (0.5, 0.5)");
    });
    
    Test("rt::geo::rotate(Ray, float) - rotate pi/2", []() {
        rt::Ray ray(rt::Point(), rt::Point(1, 0));
        rt::Ray rotated = rt::geo::rotate(ray, 3.14159 / 2.f);
        
        AssertEqual(rotated.direction.dest.x, 0, "vector should point up");
        AssertEqual(rotated.direction.dest.y, 1, "vector should point up");
    });
    
    Test("rt::geo::rotate(Ray, float) - rotate tests pi/4", []() {
        rt::Ray ray(rt::Point(), rt::Point(1, 0));
        rt::Ray rotated = rt::geo::rotate(ray, 3.14159 / 4.f);
        
        AssertEqual(rotated.direction.dest.x, sqrt(2) / 2.f, "vector should point left");
        AssertEqual(rotated.direction.dest.y, sqrt(2) / 2.f, "vector should point up");
    });
    
    Test("rt::geo::rotate(Ray, float) - rotate tests -pi/2", []() {
        rt::Ray ray(rt::Point(), rt::Point(1, 0));
        rt::Ray rotated = rt::geo::rotate(ray, - 3.14159 / 2.f);
        
        AssertEqual(rotated.direction.dest.x, 0, "vector should point down");
        AssertEqual(rotated.direction.dest.y, -1, "vector should point down");
    });
    
    Test("rt::geo::rotate(Ray, float) - rotate tests -pi/4", []() {
        rt::Ray ray(rt::Point(), rt::Point(1, 0));
        rt::Ray rotated = rt::geo::rotate(ray, -3.14159 / 4.f);
        
        AssertEqual(rotated.direction.dest.x, sqrt(2) / 2.f, "vector should point left");
        AssertEqual(rotated.direction.dest.y, - sqrt(2) / 2.f, "vector should point down");
    });
    
    Test("rt::geo::reflect(rt::Vector, rt::LineSegment)", []() {
        
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
        
        AssertEqual(reflection.direction.dest.x, -1 * sqrt(2) / 2.f, "x should be -sqrt(2)/2.f");
        AssertEqual(reflection.direction.dest.y, sqrt(2) / 2.f, "y should be sqrt(2) / 2.f");
    });
    
    Test("RayTracer::trace(Environment, Point) - sanity case", []() {
        
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

            Assert(paths.size() == 1, "There should be one path");
            Assert(paths[0].origin == rt::Point(0.f, 0.f), "This^ path should have with origin (0, 0)");
            Assert(paths[0].dest == rt::Point(-1.f, -1.f), "This^ path should have ray dest at (-1, -1)");
        }

        {
            rt::Ray startingRay({0, 0}, {0.5, 0.25});

            // Trace rays
            std::vector<rt::Vector> paths = raytracer.trace(startingRay, 1);

            Assert(paths.size() == 1, "There should be one path");
            Assert(paths[0].origin == rt::Point(0.f, 0.f), "This^ path should have with origin (0, 0)");
            Assert(paths[0].dest == rt::Point(1.f, 0.5f), "This^ path should have ray dest at (1, 0.5)");
        }
        
        {
            rt::Ray startingRay({0, 0}, {0.5, 0.25});
            
            // Trace rays
            std::vector<rt::Vector> paths = raytracer.trace(startingRay, 3);
            
            
            Assert(paths.size() == 3, "There should be one path");
            
            Assert(paths[0].origin == rt::Point(0.f, 0.f), "This path should have with origin (0, 0)");
            Assert(paths[0].dest == rt::Point(1.f, 0.5f), "This path should have ray dest at (1, 0.5)");
            Assert(paths[0].dest == paths[1].origin, "d of first ray is o of second ray");
            Assert(paths[1].dest == rt::Point(0.f, 1.f), "d of second ray is (0, 1)");
            Assert(paths[1].dest == paths[2].origin, "d of first day is o of second ray");
            Assert(paths[2].dest == rt::Point(-1, 0.5f), "d of second day is (-1, 0)");
        }
    });
    
}

void rt::test::Fail(const char *msg) {
    LOG("Assertion failed.");
    LOG(msg);
    exit(1);
}

void rt::test::Assert(bool cond, const char *msg) {
    if (!cond) {
        return Fail(msg);
    }
}

void rt::test::AssertEqual(float a, float b, const char *msg) {
    if (abs(a - b) > 0.001) {
        auto m = rt::str::format("%f != %f", a, b).c_str();
        return Fail(m);
    }
}

void rt::test::Test(const char *msg, std::function<void()> lambda) {
    rt::log(rt::str::format("➡️ %s", msg));
    lambda();
}
