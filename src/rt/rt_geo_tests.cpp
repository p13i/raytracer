#include "rt_geo.hpp"
#include "rt_linesegment.hpp"

#include "gtest.h"

using namespace rt::geo;
using namespace rt;

TEST(intersection, EndpointsShouldIntersectTrivial) {
    LineSegment a{{0, 0}, {1, 1}};
    LineSegment b{{0, 0}, {-1, -1}};

    Point intersectionPoint;
    EXPECT_TRUE(intersection(a, b, intersectionPoint));
    EXPECT_EQ(intersectionPoint, Point(0,0));
}

TEST(intersection, ShouldIntersectNotAtEndpointsTrivial) {
    LineSegment a{{-1, -1}, {1, 1}};
    LineSegment b{{-1, 1}, {1, -1}};

    Point intersectionPoint;
    EXPECT_TRUE(intersection(a, b, intersectionPoint));
    EXPECT_EQ(intersectionPoint, Point(0,0));
}

TEST(RayTest, ParallelRaysDoNotIntersect) {
    Ray ray1 = {{1, 1}, {2,2}};
    Ray ray2 = {{1,2}, {2, 3}};
    Point _;
    EXPECT_FALSE(intersection_of_rays(ray1, ray2, _));
}

TEST(RayTest, IntersectingRays) {
    Ray ray1 = {{0, 0}, Point{1, 1}};
    Ray ray2 = {{0, 1}, Point{1, 0}};
    Point intersection;
    EXPECT_TRUE(intersection_of_rays(ray1, ray2, intersection));
    EXPECT_EQ(intersection, Point(0.5, 0.5));
}
