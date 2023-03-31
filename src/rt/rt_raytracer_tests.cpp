#include <map>

#include "gtest.h"
#include "rt_math.hpp"
#include "rt_raytracer.hpp"

using namespace std;
using namespace rt;

class RayTracerTest : public testing::Test {
 protected:
  std::map<std::string, Environment> mEnvironments{
      {"first", Environment{Geometry(vector<LineSegment>{
                    // Two vertical lines, one at x=-100 and
                    // one at x=-200
                    // from y=-100 to y=100 on each line
                    {{-100, -100}, {-100, +100}},
                    {{-200, -100}, {-200, +100}},
                })}},
  };

  std::vector<Trace<Beam *> *> BeamCast(
      const std::string &environmentName) const {
    RayTracer raytracer{mEnvironments.at(environmentName)};
    Ray start{{0, 0}, {0, 1}};
    return raytracer.beamCast(start, RT_PI / 2.f, 1);
  }
};

#if 0
TEST_F(RayTracerTest, Basic) {
	auto result = BeamCast("first");
	EXPECT_EQ(1, result.size());
	auto firstTrace = result[0];
	EXPECT_TRUE(firstTrace->isTerminal);
	EXPECT_EQ(nullptr, firstTrace->next);
	EXPECT_EQ(1, firstTrace->luminance);
	EXPECT_NE(nullptr, firstTrace->data);
	Beam expectedBeam = Beam{{{0,0},{0,0}},{{0,0},{0,0}}};
	Beam beam = *firstTrace->data;
	EXPECT_EQ(expectedBeam, beam);
}
#endif

template <typename T>
std::ostream &operator<<(std::ostream &os,
                         const std::vector<T> &v) {
  using namespace std;
  os << "[" << std::endl;
  for (const T &item : v) {
    os << "    " << item << "," << std::endl;
  }
  os << "]" << std::endl;
  return os;
}

TEST_F(
    RayTracerTest,
    ProcessUnboundBeam_StartingBeamCompletelyCoversFirstIntersectedEdge) {
  Environment env = mEnvironments.at("first");

  UnboundBeam u_beam{{{0, 0}, {-50, -50}},
                     {{0, 0}, {-50, 50}}};
  auto result =
      ProcessUnboundBeam(u_beam, env.mGeometry.edges);
  LineSegment expected{{-100, -100}, {-100, 100}};
  EXPECT_TRUE(expected == result.L_closest_processed);
  EXPECT_EQ(1, result.C.size());
  EXPECT_TRUE(
      Beam({{0, 0}, {-100, -100}}, {{0, 0}, {-100, 100}}) ==
      result.C.at(0));
  EXPECT_EQ(0, result.next_u_beams.size())
      << result.next_u_beams;
}

TEST_F(
    RayTracerTest,
    ProcessUnboundBeam_MoreThanHalfPiRadiansStartingBeamSpreadShouldProduceTwoShadowBeams) {
  Environment env = mEnvironments.at("first");

  UnboundBeam u_beam{{{0, 0}, {-50, -70}},
                     {{0, 0}, {-50, 70}}};
  auto result =
      ProcessUnboundBeam(u_beam, env.mGeometry.edges);
  EXPECT_TRUE(LineSegment({-100, -100}, {-100, 100}) ==
              result.L_closest_processed);
  EXPECT_EQ(1, result.C.size());
  EXPECT_TRUE(
      Beam({{0, 0}, {-100, -100}}, {{0, 0}, {-100, 100}}) ==
      result.C.at(0));
  EXPECT_EQ(2, result.next_u_beams.size())
      << result.next_u_beams;
  std::cout << "result.next_u_beams: "
            << result.next_u_beams;
}

TEST_F(RayTracerTest,
       ProcessUnboundBeam_NextUBeamsOnlyOnOneSide) {
  Environment env = mEnvironments.at("first");

  UnboundBeam u_beam{{{0, 0}, {-50, -30}},
                     {{0, 0}, {-50, 70}}};
  auto result =
      ProcessUnboundBeam(u_beam, env.mGeometry.edges);
  EXPECT_TRUE(LineSegment({-100, -100}, {-100, 100}) ==
              result.L_closest_processed);
  EXPECT_EQ(1, result.C.size());
  EXPECT_TRUE(
      Beam({{0, 0}, {-100, -100}}, {{0, 0}, {-100, 100}}) ==
      result.C.at(0));
  EXPECT_EQ(1, result.next_u_beams.size());
  std::cout << "result.C: " << result.C;
  std::cout << "result.next_u_beams: "
            << result.next_u_beams;
}