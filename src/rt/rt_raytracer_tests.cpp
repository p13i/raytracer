#include "rt_raytracer.hpp"
#include "gtest.h"
#include <map>

using namespace std;
using namespace rt;

class RayTracerTest : public ::testing::Test {
	std::map<std::string, Environment> mEnvironments{
		{"first", Environment{Geometry(vector<LineSegment>{
		{ {-100, -100}, {-100, +100}},
		{{-200, -100}, {-200, +100}},
	}) } },
	};

protected:
	std::vector<Trace<Beam*>*> BeamCast(std::string environmentName) {
		RayTracer raytracer{ mEnvironments.at(environmentName) };
		Ray start{ {0,0}, {0,1} };
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
