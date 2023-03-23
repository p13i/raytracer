#include "rt_beam.hpp"

#include "gtest.h"

using namespace rt;

TEST(UnboundBeam, LineSegmentCompletelyInsideBeam) {
    const UnboundBeam u_beam{{{0,0},{-1,1}}, {{0,0},{1,1}}};
    const LineSegment line_segment{{-0.5,1},{0.5,1}};
    EXPECT_TRUE(Contains(u_beam, line_segment));
}

TEST(UnboundBeam, LineSegmentLeftSideInBeamRightSideOutside) {
    const UnboundBeam u_beam{{{0,0},{-1,1}}, {{0,0},{1,1}}};
    const LineSegment line_segment{{-0.5,1},{2,1}};
    EXPECT_TRUE(Contains(u_beam, line_segment));
}

TEST(UnboundBeam, LineSegmentLeftSideOutsideBeamRightSideInside) {
    const UnboundBeam u_beam{{{0,0},{-1,1}}, {{0,0},{1,1}}};
    const LineSegment line_segment{{-3,1},{0.5,1}};
    EXPECT_TRUE(Contains(u_beam, line_segment));
}

TEST(UnboundBeam, LineSegmentBothSidesOutsideButLineCrosses) {
    const UnboundBeam u_beam{{{0,0},{-1,1}}, {{0,0},{1,1}}};
    const LineSegment line_segment{{-3,1},{3,1}};
    EXPECT_TRUE(Contains(u_beam, line_segment));
}

TEST(UnboundBeam, LineSegmentOutsideBeam) {
    const UnboundBeam u_beam{{{0,0},{-1,1}}, {{0,0},{1,1}}};
    const LineSegment line_segment{{-1,-1},{1,-1}};
    EXPECT_FALSE(Contains(u_beam, line_segment));
}
