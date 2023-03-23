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
