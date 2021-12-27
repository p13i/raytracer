#ifndef raytracer_hpp
#define raytracer_hpp


#include "rt_geometry.hpp"
#include "rt_linesegment.hpp"
#include "rt_vector.hpp"
#include "rt_trace.hpp"
#include "rt_ray.hpp"
#include "rt_environment.hpp"
#include "rt_settings.hpp"
#include "rt_beam.hpp"

#include <utility>
#include <vector>

using namespace std;

namespace rt {

class RayTracer {
public:
    Environment mEnvironment;
    RayTracer(): RayTracer({{}}) {};
    RayTracer(Environment environment): mEnvironment(std::move(environment)) {}
    Trace<Vector>* trace(Ray start, unsigned int depth = 1);
    vector<Trace<Vector>*> cast(Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth = 1);
    vector<Trace<Beam*>*> beamCast(Ray start, float spreadRadians, unsigned int depth = 1);
};

}


#endif /* raytracer_hpp */
